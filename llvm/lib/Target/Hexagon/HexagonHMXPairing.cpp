//===- HexagonHMXPairing.cpp - HMX activation/weight pairing reorder -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Reorders HMX activation and weight mxmem instructions so they are adjacent
// in the same block, allowing the packetizer to place them in one packet.
//
// When act and wei are not adjacent, the post-RA scheduler may have placed
// an in-between def whose target register collides with act's reads (e.g.
// $r0 = $r3 between act and wei where act also reads $r0). The pre-fix
// pass renamed act's use and silently dropped the in-between def, leaving
// a downstream consumer (such as a memcpy call) reading a stale value of
// $r0. This pass instead splices each conflict-causing in-between def
// past wei, preserving the original def's destination register so the
// downstream consumer still observes it. Activation reads the start-of-
// packet value of the conflict reg (the semantically intended behavior),
// and wei is unaffected because it does not consume those defs.
//
// For any in-between instructions that wei *does* consume (rare; the
// canonical case is a Rt-immediate reload shared between act and wei via
// the same physical register), the pass falls back to a register rename:
// pick a scratch reg that is free across the activation point, rewrite
// the in-between def and wei's matching use, then splice act adjacent
// to wei.
//
//===----------------------------------------------------------------------===//

#include "Hexagon.h"
#include "HexagonInstrInfo.h"
#include "HexagonRegisterInfo.h"
#include "HexagonSubtarget.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include <algorithm>
#include <cassert>
#include <iterator>

using namespace llvm;

namespace llvm {

FunctionPass *createHexagonHMXPairing();
void initializeHexagonHMXPairingPass(PassRegistry &);

} // end namespace llvm

namespace {

static bool reorderHMXMxmemInBlock(MachineBasicBlock &MBB,
                                   MachineFunction &MF,
                                   const HexagonInstrInfo *HII,
                                   const HexagonRegisterInfo *HRI) {
  SmallVector<MachineBasicBlock::iterator, 4> Acts, Weights;
  for (MachineBasicBlock::iterator It = MBB.begin(), E = MBB.end(); It != E;
       ++It) {
    if (HII->isHMXActivationMxmem(*It))
      Acts.push_back(It);
    else if (HII->isHMXWeightMxmem(*It))
      Weights.push_back(It);
  }
  if (Acts.empty() || Weights.empty())
    return false;
  bool Changed = false;
  const MCPhysReg ScratchCandidates[] = {
      Hexagon::R6,  Hexagon::R7,  Hexagon::R8,  Hexagon::R9,
      Hexagon::R10, Hexagon::R11, Hexagon::R12, Hexagon::R13,
      Hexagon::R14, Hexagon::R15};
  for (int P = (int)std::min(Acts.size(), Weights.size()) - 1; P >= 0; --P) {
    MachineBasicBlock::iterator ActIt = Acts[P];
    MachineBasicBlock::iterator WIt = Weights[P];
    MachineInstr &Weight = *WIt;
    MachineInstr &Act = *ActIt;
    auto NextAfterAct = std::next(ActIt);
    if (NextAfterAct == WIt)
      continue;
    assert(WIt != ActIt && "weight must be after activation in program order");

    // Track the *actual* operand registers of act / weight (no alias
    // expansion). Alias relationships between, say, a pair register D0
    // = R1:R0 and the single R0 are handled on the DEF side instead —
    // when an in-between instruction defines such a register, we
    // expand its aliases and check whether any of them collides with
    // act's actual reads. Expanding ActUses with aliases the way the
    // first version of this pass did inflates the set with non-uses
    // (e.g. W0 = R3:R2:R1:R0 ⊇ R0) and produced spurious renames of
    // wider register classes than act actually reads, breaking class
    // compatibility at Step 3.
    SmallSet<Register, 8> WeightUses, WeightDefs, ActUses, ActDefs;
    for (const MachineOperand &MO : Weight.operands()) {
      if (!MO.isReg() || !MO.getReg())
        continue;
      if (MO.isUse())
        WeightUses.insert(MO.getReg());
      else if (MO.isDef())
        WeightDefs.insert(MO.getReg());
    }
    for (const MachineOperand &MO : Act.operands()) {
      if (!MO.isReg() || !MO.getReg())
        continue;
      if (MO.isUse())
        ActUses.insert(MO.getReg());
      else if (MO.isDef())
        ActDefs.insert(MO.getReg());
    }

    // Returns true if `R` (or any register that aliases it) is in the
    // given set `S`. Used to detect conflicts where an in-between def
    // (potentially a pair register) covers act's actual single-register
    // read.
    auto RegOrAliasIn = [&](Register R, const SmallSet<Register, 8> &S) {
      if (S.count(R))
        return true;
      if (!R.isPhysical())
        return false;
      for (MCRegAliasIterator AI(R.asMCReg(), HRI, true); AI.isValid(); ++AI)
        if (S.count(*AI))
          return true;
      return false;
    };

    // Step 1: Move conflict-only in-between defs past wei.
    //
    // An in-between instruction is moved when:
    //   (a) it defines a register that activation also reads (it would
    //       otherwise force a rename of act's use), AND
    //   (b) weight does not read any of the instruction's defs (otherwise
    //       moving it past wei would change wei's input), AND
    //   (c) the instruction has no unmodelled side effects (no calls,
    //       no stores, no volatile loads). Plain loads (e.g. spill
    //       reloads of an ABI call argument such as memcpy's r0=%dst)
    //       are explicitly *allowed* to move: HMX activation/weight do
    //       not write any addressable memory, so loads from stack or
    //       VTCM are independent of the HMX state and safe to relocate.
    //
    // The canonical cases:
    //   (i)  the post-RA scheduler placing a call-arg setup like
    //        `$r0 = $r3` between act and wei where $r0 is also
    //        activation's first operand;
    //   (ii) a spill-reload such as `$r0 = L2_loadri_io $r29, <off>`
    //        materialising the next call's r0 argument between act and
    //        wei (variant A pattern: memcpy(%C, %C_global_vtcm, ...)
    //        right after the HMX MAC + drain).
    // Pre-fix the pass renamed act's $r0 read in both cases and broke
    // the downstream consumer (memcpy reads a stale or renamed r0).
    // Moving the in-between def past wei preserves the destination
    // register so downstream uses observe the correct value. Hexagon
    // packet semantics ensure act still reads the START-of-packet
    // value when the packetizer later bundles the moved transfer into
    // act+wei's packet.
    // Step 1: Splice movable in-between defs past wei (and past any
    // isSolo drain that follows wei). An in-between instruction is
    // moved only when:
    //   (a) it has no unmodelled side effects (no calls, inline asm),
    //   (b) wei does NOT read any of its defs (otherwise moving it
    //       past wei would change wei's input),
    //   (c) it does not access memory (mayLoadOrStore() == false), and
    //   (d) at least one of its defs (or aliases) collides with act's
    //       reads (otherwise leaving it in place is fine; act+wei can
    //       be paired by renaming the conflict register in Step 3
    //       below, and Step 1 should not gratuitously reorder code
    //       across the splice point — doing so was observed to break
    //       N-tile outer loops where unrelated stack reloads carry
    //       state across loop iterations).
    {
      MachineBasicBlock::iterator I = NextAfterAct;
      while (I != WIt) {
        auto Next = std::next(I);
        if (I->isCall() || I->hasUnmodeledSideEffects() ||
            I->mayLoadOrStore()) {
          I = Next;
          continue;
        }
        bool DefsReadByWei = false;
        bool DefsConflictWithAct = false;
        for (const MachineOperand &MO : I->operands()) {
          if (!MO.isReg() || !MO.getReg() || !MO.isDef())
            continue;
          if (RegOrAliasIn(MO.getReg(), WeightUses)) {
            DefsReadByWei = true;
            break;
          }
          if (RegOrAliasIn(MO.getReg(), ActUses))
            DefsConflictWithAct = true;
        }
        if (!DefsReadByWei && DefsConflictWithAct) {
          auto InsertPos = std::next(WIt);
          if (InsertPos != MBB.end() && HII->isSolo(*InsertPos))
            ++InsertPos;
          MBB.splice(InsertPos, &MBB, I);
        }
        I = Next;
      }
    }

    // Step 2: Recompute conflicts. After step 1 the only in-between code
    // left is instructions that wei depends on. Their def-regs may still
    // collide with act's uses (the canonical case: an Rt-immediate redef
    // shared between act and wei via the same physical register, e.g.
    // `$r5 = 124; act(...,$r5); $r5 = 1920; wei(...,$r5)`).
    //
    // NB: re-derive `NextAfterAct` from `ActIt` here. The original
    // NextAfterAct iterator captured before step 1 may now point past
    // WIt (the original "first in-between" instruction may have been
    // spliced past wei). Walking from a stale NextAfterAct toward WIt
    // would walk off the basic block and crash.
    NextAfterAct = std::next(ActIt);
    // Conflict-set entries are in-between def-regs whose write (or
    // alias write — e.g. a pair-register def covering act's
    // single-register read) clobbers something act needs to read. We
    // keep entries in the in-between's *own* class so the scratch
    // picker chooses a same-class scratch and the restore COPY is
    // class-compatible. Each entry records both the def-reg to rename
    // and the act-use it covers so we know what to restore.
    struct Conflict {
      Register DefReg;    // the in-between's def reg (to be renamed)
      Register ActUseReg; // the act-use covered by DefReg (to be restored)
    };
    SmallVector<Conflict, 4> ConflictSet;
    SmallSet<Register, 4> SeenDefs;
    for (MachineBasicBlock::iterator Between = NextAfterAct; Between != WIt;
         ++Between) {
      for (const MachineOperand &MO : Between->operands()) {
        if (!MO.isReg() || !MO.getReg() || !MO.isDef())
          continue;
        Register D = MO.getReg();
        if (!SeenDefs.insert(D).second)
          continue;
        // Find the first act-use that this def or one of its aliases
        // clobbers, then record (D → ActUse).
        Register Covered;
        if (ActUses.count(D)) {
          Covered = D;
        } else if (D.isPhysical()) {
          for (MCRegAliasIterator AI(D.asMCReg(), HRI, true); AI.isValid();
               ++AI) {
            if (ActUses.count(*AI)) {
              Covered = *AI;
              break;
            }
          }
        }
        if (Covered)
          ConflictSet.push_back({D, Covered});
      }
    }

    // Pick a scratch register for rename. The chosen register must not
    // be defined or used anywhere from the activation position to the
    // end of the basic block; otherwise some later instruction would
    // either clobber the renamed value or be clobbered by us. The pre-
    // fix pass only checked against {act,wei,in-between} operand sets
    // and could choose a register that was live across the splice
    // region — e.g. r6 holding a drain Rt = 0 immediate that subsequent
    // code still needed.
    // True iff `R` is dead from `From` (inclusive) through the restore
    // insert position (just past wei + any isSolo drain that follows).
    // We pick the scratch to be unused only in the small region between
    // the in-between def and the restore copy that re-establishes the
    // original physical register. Outside that window the scratch is
    // free (the restore copy writes the original reg back, killing the
    // scratch's logical value), so demanding BB-wide deadness was
    // strictly more restrictive than needed and failed to find a
    // scratch in moderately register-pressured loops (e.g. N>=64 HMX
    // tile loops with several extra V6/L2 reloads live across the
    // splice region).
    auto RestoreLimit = [&]() {
      auto Limit = std::next(WIt);
      if (Limit != MBB.end() && HII->isSolo(*Limit))
        ++Limit;
      return Limit;
    };
    auto isFreeInRange = [&](Register R, MachineBasicBlock::iterator From,
                             MachineBasicBlock::iterator To) {
      for (auto I = From; I != To; ++I) {
        for (const MachineOperand &MO : I->operands()) {
          if (MO.isReg() && MO.getReg() && MO.getReg() == R)
            return false;
        }
      }
      return true;
    };

    DenseMap<Register, Register> RenameMap;        // def-reg → scratch
    DenseMap<Register, Register> RestoreMap;       // act-use → scratch
    SmallSet<Register, 8> UsedRegs(ActUses);
    UsedRegs.insert(WeightUses.begin(), WeightUses.end());
    for (MachineBasicBlock::iterator Between = NextAfterAct; Between != WIt;
         ++Between) {
      for (const MachineOperand &MO : Between->operands())
        if (MO.isReg() && MO.getReg() && MO.isDef())
          UsedRegs.insert(MO.getReg());
    }
    for (const Conflict &C : ConflictSet) {
      // Scratch must be in the same register class as the in-between
      // def we're renaming. We only stock ScratchCandidates with
      // single-GPR registers (R6..R15), so refuse to pick a scratch
      // when the def is a wider class. `IntRegs` is the catch-all
      // 32-bit GPR class; the minimal class for a specific physreg
      // might be a narrower subset (e.g. IntRegsLow8 for R0..R7),
      // so check class membership rather than class identity.
      if (!C.DefReg.isPhysical() ||
          !Hexagon::IntRegsRegClass.contains(C.DefReg.asMCReg()))
        continue;
      Register ScratchR;
      auto RestoreLim = RestoreLimit();
      for (MCPhysReg Cr : ScratchCandidates) {
        Register CrReg = Cr;
        if (UsedRegs.count(CrReg))
          continue;
        if (!isFreeInRange(CrReg, ActIt, RestoreLim))
          continue;
        ScratchR = CrReg;
        UsedRegs.insert(CrReg);
        break;
      }
      if (!ScratchR)
        break;
      RenameMap[C.DefReg] = ScratchR;
      RestoreMap[C.ActUseReg] = ScratchR;
    }

    if (!ConflictSet.empty() && RenameMap.size() != ConflictSet.size()) {
      // Could not rename every remaining conflict. Bail out without
      // splicing rather than emit an incomplete rename. The packetizer
      // will assert on non-adjacent act/wei and the user will see a
      // clear failure (rather than a silent miscompile).
      continue;
    }

    // Step 3: Apply renames to in-between defs and to wei's uses, in
    // place. We modify operand registers directly rather than rebuilding
    // the instruction with BuildMI: rebuilding loses operand flags
    // (kill / dead / implicit / metadata-tracking) and was observed to
    // trigger use-after-free in MetadataTracking when the original
    // instruction was erased after MIB.add() shallow-copied its
    // operands.
    //
    // Each renamed in-between def carries a value that downstream code
    // expected to find in the *original* register (the canonical case
    // is a `$r0 = L2_loadri_io` reload setting up memcpy's ABI argument
    // r0). Renaming the def alone leaves the downstream consumer
    // reading whatever pre-renamed value happened to be in the
    // original register (usually the HMX Rt constant) — the very bug
    // we are trying to avoid. After applying the renames we emit a
    // restore copy `$orig = COPY $scratch` immediately after wei so
    // the original register is re-established before any later use.
    // The restore copy is a single-cycle transfer that the Hexagon
    // packetizer is free to bundle into the call's packet
    // (call-arg-setup pattern: `{ $r0 = $r6 ; call memcpy }`), which
    // is exactly the lit-test-validated shape.
    for (MachineBasicBlock::iterator Between = NextAfterAct; Between != WIt;
         ++Between) {
      for (MachineOperand &MO : Between->operands()) {
        if (!MO.isReg() || !MO.getReg() || !MO.isDef())
          continue;
        auto It = RenameMap.find(MO.getReg());
        if (It != RenameMap.end())
          MO.setReg(It->second);
      }
    }
    for (MachineOperand &MO : Weight.operands()) {
      if (!MO.isReg() || !MO.getReg() || !MO.isUse())
        continue;
      auto It = RenameMap.find(MO.getReg());
      if (It != RenameMap.end())
        MO.setReg(It->second);
    }

    MBB.splice(WIt, &MBB, ActIt);

    // Emit restore copies: `$orig = COPY $scratch` after wei, for
    // every conflict we renamed. The COPY targets the act-use
    // register so downstream consumers (memcpy ABI args, future HMX
    // Rt, etc.) read the value the in-between def originally placed
    // there.
    //
    // Insert *past* any isSolo instruction that follows wei
    // (typically the HMX drain mxcvtr.sat.hf). The Hexagon packetizer
    // is otherwise free to pull the restore COPY back up into the
    // act+wei packet — and although that's slot-valid for the
    // 2-HMX-ops + 1-transfer shape the lit test exercises, more
    // complex pre-act regions (with pre-act loads competing for the
    // memory slots) hit a packetizer "slot error" at MC emit time.
    // Placing the restore *after* the solo drain forces the COPY
    // into the call's packet — the Hexagon `{ $r0 = $r3 ; call X }`
    // ABI-setup idiom — which the packetizer handles cleanly and
    // which is consistent with how the lit test prefers the layout
    // anyway.
    auto RestoreInsertPos = std::next(WIt);
    if (RestoreInsertPos != MBB.end() && HII->isSolo(*RestoreInsertPos))
      ++RestoreInsertPos;
    for (auto &KV : RestoreMap) {
      Register OrigReg = KV.first;
      Register ScratchReg = KV.second;
      // Use A2_tfr (concrete Hexagon transfer) rather than the generic
      // TargetOpcode::COPY pseudo. This pass runs after Post-RA Pseudo
      // Expansion, so a TargetOpcode::COPY inserted here would survive
      // unexpanded; the packetizer / MC shuffler don't have correct
      // resource modelling for pseudo COPYs and emit a spurious slot
      // error when packing the unexpanded COPY into the call packet.
      BuildMI(MBB, RestoreInsertPos, WIt->getDebugLoc(),
              HII->get(Hexagon::A2_tfr), OrigReg)
          .addReg(ScratchReg);
    }
    Changed = true;
  }
  return Changed;
}

struct HexagonHMXPairing : public MachineFunctionPass {
  static char ID;

  HexagonHMXPairing() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override {
    return "Hexagon HMX Pairing";
  }

  bool runOnMachineFunction(MachineFunction &MF) override {
    const auto &HST = MF.getSubtarget<HexagonSubtarget>();
    const HexagonInstrInfo *HII = HST.getInstrInfo();
    const HexagonRegisterInfo *HRI = HST.getRegisterInfo();
    bool Changed = false;
    for (MachineBasicBlock &MBB : MF)
      Changed |= reorderHMXMxmemInBlock(MBB, MF, HII, HRI);
    return Changed;
  }

  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::NoVRegs);
  }
};

} // end anonymous namespace

char HexagonHMXPairing::ID = 0;

INITIALIZE_PASS_BEGIN(HexagonHMXPairing, "hexagon-hmx-pairing",
                      "Hexagon HMX pairing", false, false)
INITIALIZE_PASS_END(HexagonHMXPairing, "hexagon-hmx-pairing",
                    "Hexagon HMX pairing", false, false)

FunctionPass *llvm::createHexagonHMXPairing() {
  return new HexagonHMXPairing();
}
