# V79 QF transport correctness prototype

This experimental branch preserves extended register state in compiler-generated
HVX COPY and spill transport when V79 HVX and QF operations are enabled. It does
not change floating-point arithmetic or explicit raw VMEM/vassign intrinsics.
It is not approved for production integration.

Generated single-vector COPY uses `vmerge(src.x, src.w)`. Vector spills retain
their original data frame object and add a separate, vector-aligned sideband
spill object for extended bits. Pair spills have one sideband vector per data
half. Expansion occurs in `expandSpillMacros`, after StackSlotColoring and before
frame layout. Both data and sideband have their own exact MMO and actual
alignment-based VMEM selection.

V31 and its overlapping pair aliases are reserved to avoid recursive vector
scavenging. Mask temporaries use the existing IntRegs/NewRegs scavenger path.
Predicate spill helpers also use V31. QF-mode vector spill-slot register
promotion is conservatively disabled; integer/vector code can therefore incur
extra register pressure, stack, and instruction costs. No performance benefit
is claimed.

Current evidence:

- The independent, unchanged allocator-pressure fixture (`norm-pressure.ll`,
  SHA `590ed099e20ef668edcd7571e7dc7d44eaa46fe345b446f8b8fa4ec3911ccc3a`)
  changed from 252 mismatches to zero in three X device sessions.
- This compares newly compiled retained and pressure paths; cross-version
  preservation of the old retained path is a separate pending control.
- `qfloat-state-spill-frame.mir` verifies single/pair transport, non-kill source
  restoration, mixed-class frame reuse, a dominating-aligna VLA case, and
  clamped unaligned stack access. QF/no-QF variants pass machine verification
  and FileCheck.
- Ten existing HF multiplication, masked-store, HMX packetization, stack, and
  scalar host checks pass. Device HF/HMX/integer regressions remain pending.

Known limits before wider adoption:

- Reversed WR destination encoding is not yet handled correctly. TableGen
  subregister order differs from its hardware destination order. These aliases
  are already excluded from normal register allocation, but direct COPY MIR
  requires a separate fix and regression.
- The VLA test proves only that a dominating PS_aligna provides the same base
  for primary and sideband slots. It does not prove general AP lifetime safety.
- Full FP16/integer device regression, 64-byte V79 transport coverage, and full
  Qwen correctness are not established. The existing original reference and its
  fullbody385 failure status are unchanged.

The frozen original decoder independently exhibited a compiler-generated raw
QF spill that lost extended state. A separate one-boundary corrected diagnostic
made its layer4 context, gated product, and block output raw-exact to the native
candidate. That diagnostic is evidence about one causal boundary, not a new
oracle or a production compatibility policy.
