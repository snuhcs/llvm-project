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
// If activation and weight are not adjacent, optionally renames registers to
// avoid WAR/WAW when the same physical register is defined between them and
// used by activation, then moves activation to immediately before weight.
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

    SmallSet<Register, 4> DefsBetween;
    for (MachineBasicBlock::iterator Between = NextAfterAct; Between != WIt;
         ++Between) {
      for (const MachineOperand &MO : Between->operands())
        if (MO.isReg() && MO.getReg() && MO.isDef())
          DefsBetween.insert(MO.getReg());
    }

    // ConflictSet = DefsBetween ∩ ActUses (regs defined between act/weight and
    // used by act; reordering would cause WAR/WAW). Rename all of them.
    SmallVector<Register, 4> ConflictSet;
    for (Register R : ActUses)
      if (DefsBetween.count(R))
        ConflictSet.push_back(R);

    DenseMap<Register, Register> RenameMap;
    SmallSet<Register, 8> UsedRegs(ActUses);
    UsedRegs.insert(WeightUses.begin(), WeightUses.end());
    UsedRegs.insert(DefsBetween.begin(), DefsBetween.end());
    for (Register R : ConflictSet) {
      Register ScratchR;
      for (MCPhysReg C : ScratchCandidates) {
        Register Cr = C;
        if (!UsedRegs.count(Cr)) {
          ScratchR = Cr;
          UsedRegs.insert(Cr);
          break;
        }
      }
      if (!ScratchR)
        break;
      RenameMap[R] = ScratchR;
    }

    if (RenameMap.size() == ConflictSet.size()) {
      for (MachineBasicBlock::iterator Between = NextAfterAct; Between != WIt;) {
        MachineInstr &I = *Between;
        auto NextBetween = std::next(Between);
        MachineInstrBuilder MIB =
            BuildMI(MF, I.getDebugLoc(), HII->get(I.getOpcode()));
        for (const MachineOperand &MO : I.operands()) {
          if (MO.isReg() && MO.getReg() && MO.isDef()) {
            auto It = RenameMap.find(MO.getReg());
            MIB.addReg(It != RenameMap.end() ? It->second : MO.getReg());
          } else
            MIB.add(MO);
        }
        MBB.insert(Between, MIB);
        I.eraseFromParent();
        Between = NextBetween;
      }
      for (MachineOperand &MO : Weight.operands()) {
        if (!MO.isReg() || !MO.getReg() || !MO.isUse())
          continue;
        auto It = RenameMap.find(MO.getReg());
        if (It != RenameMap.end())
          MO.setReg(It->second);
      }
    }

    MBB.splice(WIt, &MBB, ActIt);
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
