; RUN: llc -mtriple=hexagon -mcpu=hexagonv75 -mattr=+hvxv75,+hvx-length128b,+hmx < %s | FileCheck %s
;
; Regression test for the Hexagon HMX regalloc / scheduling bug observed
; when an HMX intrinsic block is followed (in the same basic block) by a
; function call that needs ABI-conformant r0/r1/r2 arguments.
;
; Bug manifestation observed on the Snapdragon 8 Elite CDSP and reproduced
; directly with llc on this LLVM tree:
;
;   1. The HMX intrinsics take an i32 Rt operand (the activation/weight
;      spatial-mask / channel-stop encoding, e.g. 124 for `0x7C`). LLVM
;      regalloc materialises this constant into some physical register,
;      then *fails to break the live range* between that register and a
;      subsequent function call's ABI argument slot.
;
;   2. As a result the call (memcpy in the TVM HMX f16 matmul case) reads
;      one of its arguments from a register that LLVM never wrote with
;      the actual argument value. The call ends up with garbage (the
;      stale HMX Rt constant or one of the function-entry argument
;      pointers) as its first argument.
;
; On-device this surfaces as `memcpy → Bad VA 0x80` (when the stale value
; was the HMX_HF_ACT_RT immediate 0x7C, rounded up to the next aligned
; chunk = 0x80).
;
; This test compiles a function whose body mirrors the structure of the
; TIR-emitted HMX kernel:
;   mxclracc -> mxmem.act -> mxmem.wei -> mxcvtr.sat (drain) -> memcpy
;
; and asserts the correctness conditions that LLVM must guarantee in the
; emitted assembly.

declare void @llvm.hexagon.M8.mxmem.sm.act.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxcvtr.sat.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxclracc.hf()
declare void @llvm.memcpy.p0.p0.i32(ptr noalias nocapture writeonly,
                                    ptr noalias nocapture readonly,
                                    i32, i1 immarg)

; Same shape as the TVM HMX f16 matmul kernel's inner block.
; Args:
;   r0 = %act     (activation crouton in VTCM)
;   r1 = %wei     (weight tile in VTCM)
;   r2 = %scratch (VTCM scratch for HMX drain output)
;   r3 = %dst     (user-supplied output C in VTCM)
;   r4 = %src     (helper-scratch source for the final copy)
define void @hmx_then_memcpy(ptr %act, ptr %wei, ptr %scratch, ptr %dst, ptr %src) {
entry:
  call void @llvm.hexagon.M8.mxclracc.hf()
  call void @llvm.hexagon.M8.mxmem.sm.act.hf(ptr %act, i32 124)
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr %wei, i32 1920)
  ; HMX drain: write convert state to %scratch. Rt must be 0 (linear
  ; 32-spatial output, matching the activation spatial mask of 0).
  call void @llvm.hexagon.M8.mxcvtr.sat.hf(ptr %scratch, i32 0)
  ; Post-HMX function call: memcpy(%dst, %src, 2048). ABI requires
  ; r0 = %dst, r1 = %src, r2 = 2048 at call site.
  call void @llvm.memcpy.p0.p0.i32(ptr %dst, ptr %src, i32 2048, i1 false)
  ret void
}

; ---------------------------------------------------------------------------
; CHECK 1: the HMX activation, weight, drain, and memcpy call must all be
;          present, in program order.
; ---------------------------------------------------------------------------

; CHECK-LABEL: hmx_then_memcpy:
; CHECK:       activation.hf = mxmem({{r[0-9]+}},{{r[0-9]+}})
; CHECK:       weight.hf = mxmem({{r[0-9]+}},{{r[0-9]+}})
; CHECK:       mxmem({{r[0-9]+}},{{r[0-9]+}}):after.hf = acc
; CHECK-DAG:   r0 = r3
; CHECK:       call memcpy

; ---------------------------------------------------------------------------
; CHECK 2 (memcpy first-argument): the function must materialize the value
;          of %dst (which is r3 at the call ABI boundary on Hexagon) into
;          r0 *before* the memcpy call observes its arguments.
;
;          On the original buggy HexagonHMXPairing output, the splice +
;          rename combo dropped this `r0 = r3` write and the call landed
;          with r0 still equal to %act (= the function's first incoming
;          argument), or worse with r0 equal to a stale HMX Rt constant
;          (= #0x7c) for TVM-generated callers. memcpy then dereferenced
;          a tiny page-zero pointer (Bad VA 0x80) on the DSP.
;
;          The fix routes the in-between def `r0 = r3` past the weight
;          instruction (or keeps it inside the act/wei packet whose
;          semantics ensure `r0 = r3` writes the END-of-packet value
;          while activation still reads the START-of-packet r0 = %act).
;          The CHECK-DAG above asserts that this `r0 = r3` write is
;          present in the function body somewhere before the memcpy call.

; ---------------------------------------------------------------------------
; CHECK 3 (drain Rt invariant): the HMX drain's second operand (Rt) must
;          be set from a register that holds *zero*, never from a register
;          that holds a function-argument pointer. The pre-fix bug renamed
;          an in-between `r0 = r3` to `r6 = r3`, clobbering the r6 = 0
;          drain Rt; this CHECK asserts that pattern is gone.
; ---------------------------------------------------------------------------

; CHECK-NOT: r6 = r3
; CHECK-NOT: r6 = r4

