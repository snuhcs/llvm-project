//===- HexagonHMXPairing.cpp - HMX act+wei glue and expand ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Hardware requires HMX activation and weight mxmem in the same VLIW packet.
// Post-RA shuffling of the two MIs across reused physregs (X-A/X-C/X-D)
// rewrites loop latches onto HMX Rt or call ABI values.
//
// Pre-RA (SSA): splice activation down onto its weight (vregs keep their
// identity) and replace the pair with PS_hmx_actwei so RA / post-RA sched
// see one instruction.
//
// PreEmit: expand the pseudo back to adjacent real mxmem for the packetizer.
// Do not expand in expandPostRAPseudo (that runs before post-RA sched).
//
//===----------------------------------------------------------------------===//

#include "Hexagon.h"
#include "HexagonInstrInfo.h"
#include "HexagonSubtarget.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"

#include <algorithm>
#include <iterator>
#include "HexagonSubtarget.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"

using namespace llvm;

namespace llvm {
FunctionPass *createHexagonHMXPairing();
FunctionPass *createHexagonMergeActivateWeight();
void initializeHexagonHMXPairingPass(PassRegistry &);
void initializeHexagonMergeActivateWeightPass(PassRegistry &);
} // end namespace llvm

namespace {

static bool isActBeforeWei(MachineBasicBlock &MBB, MachineInstr *Act,
                           MachineInstr *Wei) {
  for (MachineInstr &MI : MBB) {
    if (&MI == Act)
      return true;
    if (&MI == Wei)
      return false;
  }
  return false;
}

static bool glueHMXActWeiInBlock(MachineBasicBlock &MBB,
                                 const HexagonInstrInfo *HII) {
  SmallVector<MachineInstr *, 8> Acts, Weights;
  for (MachineInstr &MI : MBB) {
    if (HII->isHMXActivationMxmem(MI))
      Acts.push_back(&MI);
    else if (HII->isHMXWeightMxmem(MI))
      Weights.push_back(&MI);
  }
  if (Acts.empty() || Weights.empty())
    return false;

  bool Changed = false;
  int N = (int)std::min(Acts.size(), Weights.size());
  for (int P = N - 1; P >= 0; --P) {
    MachineInstr *Act = Acts[P];
    MachineInstr *Wei = Weights[P];
    if (Act->getParent() != &MBB || Wei->getParent() != &MBB)
      continue;
    if (!isActBeforeWei(MBB, Act, Wei))
      continue;
    if (Act->getNumExplicitOperands() < 2 || Wei->getNumExplicitOperands() < 2)
      continue;
    if (!Act->getOperand(0).isReg() || !Act->getOperand(1).isReg() ||
        !Wei->getOperand(0).isReg() || !Wei->getOperand(1).isReg())
      continue;

    MachineBasicBlock::iterator WIt = Wei->getIterator();
    MachineBasicBlock::iterator ActIt = Act->getIterator();
    if (std::next(ActIt) != WIt)
      MBB.splice(WIt, &MBB, ActIt);

    DebugLoc DL = Act->getDebugLoc();
    MachineInstrBuilder MIB =
        BuildMI(MBB, WIt, DL, HII->get(Hexagon::PS_hmx_actwei));
    MIB.add(Act->getOperand(0));
    MIB.add(Act->getOperand(1));
    MIB.add(Wei->getOperand(0));
    MIB.add(Wei->getOperand(1));
    MIB.addImm(Act->getOpcode());
    MIB.addImm(Wei->getOpcode());
    SmallVector<MachineMemOperand *, 4> MMOs;
    MMOs.append(Act->memoperands_begin(), Act->memoperands_end());
    MMOs.append(Wei->memoperands_begin(), Wei->memoperands_end());
    if (!MMOs.empty())
      MIB.setMemRefs(MMOs);

    Act->eraseFromParent();
    Wei->eraseFromParent();
    Changed = true;
  }
  return Changed;
}

static bool expandHMXActWeiInBlock(MachineBasicBlock &MBB,
                                   const HexagonInstrInfo *HII) {
  bool Changed = false;
  for (MachineBasicBlock::iterator I = MBB.begin(), E = MBB.end(); I != E;) {
    MachineInstr &MI = *I++;
    if (!HII->isHMXActWeiPseudo(MI))
      continue;
    assert(MI.getNumExplicitOperands() >= 6 && "PS_hmx_actwei operands");
    unsigned ActOpc = static_cast<unsigned>(MI.getOperand(4).getImm());
    unsigned WeiOpc = static_cast<unsigned>(MI.getOperand(5).getImm());
    DebugLoc DL = MI.getDebugLoc();

    auto AddUse = [](MachineInstrBuilder &B, const MachineOperand &MO,
                     bool KeepKill) {
      unsigned St = getRegState(MO);
      if (!KeepKill)
        St &= ~RegState::Kill;
      B.addReg(MO.getReg(), St, MO.getSubReg());
    };

    Register RsA = MI.getOperand(0).getReg();
    Register RtA = MI.getOperand(1).getReg();
    Register RsW = MI.getOperand(2).getReg();
    Register RtW = MI.getOperand(3).getReg();

    MachineInstrBuilder ActB =
        BuildMI(MBB, MI.getIterator(), DL, HII->get(ActOpc));
    AddUse(ActB, MI.getOperand(0), RsA != RsW && RsA != RtW);
    AddUse(ActB, MI.getOperand(1), RtA != RsW && RtA != RtW);

    MachineInstrBuilder WeiB =
        BuildMI(MBB, MI.getIterator(), DL, HII->get(WeiOpc));
    AddUse(WeiB, MI.getOperand(2), true);
    AddUse(WeiB, MI.getOperand(3), true);

    SmallVector<MachineMemOperand *, 4> MMOs(MI.memoperands_begin(),
                                             MI.memoperands_end());
    if (!MMOs.empty()) {
      ActB.setMemRefs(MMOs);
      WeiB.setMemRefs(MMOs);
    }
    MI.eraseFromParent();
    Changed = true;
  }
  return Changed;
}

} // end anonymous namespace

class HexagonMergeActivateWeight : public MachineFunctionPass {
public:
  static char ID;

  HexagonMergeActivateWeight() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override {
    return "Hexagon HMX merge activate/weight";
  }

  bool runOnMachineFunction(MachineFunction &MF) override {
    const auto &HST = MF.getSubtarget<HexagonSubtarget>();
    if (!HST.useHMXOps())
      return false;
    const HexagonInstrInfo *HII = HST.getInstrInfo();
    bool Changed = false;
    for (MachineBasicBlock &MBB : MF)
      Changed |= glueHMXActWeiInBlock(MBB, HII);
    return Changed;
  }
};

class HexagonHMXPairing : public MachineFunctionPass {
public:
  static char ID;

  HexagonHMXPairing() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override {
    return "Hexagon HMX act+wei expand";
  }

  bool runOnMachineFunction(MachineFunction &MF) override {
    const auto &HST = MF.getSubtarget<HexagonSubtarget>();
    const HexagonInstrInfo *HII = HST.getInstrInfo();
    bool Changed = false;
    for (MachineBasicBlock &MBB : MF)
      Changed |= expandHMXActWeiInBlock(MBB, HII);
    return Changed;
  }
};

char HexagonMergeActivateWeight::ID = 0;
char HexagonHMXPairing::ID = 0;

INITIALIZE_PASS(HexagonMergeActivateWeight, "hexagon-merge-activate-weight",
                "Hexagon HMX merge activate/weight", false, false)
INITIALIZE_PASS(HexagonHMXPairing, "hexagon-hmx-pairing",
                "Hexagon HMX act+wei expand", false, false)

FunctionPass *llvm::createHexagonMergeActivateWeight() {
  return new HexagonMergeActivateWeight();
}

FunctionPass *llvm::createHexagonHMXPairing() {
  return new HexagonHMXPairing();
}
