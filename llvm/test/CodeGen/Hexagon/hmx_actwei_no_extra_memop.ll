; RUN: llc -mtriple=hexagon -mcpu=hexagonv75 -mattr=+hvxv75,+hvx-length128b,+hvx-qfloat,+hmx < %s | FileCheck %s
;
; Regression test for a Hexagon packetizer slot-error when the HMX
; activation+weight pair is followed by additional scalar memory ops.
;
; Background: the Hexagon V75 packet has two memory slots (S0/S1).
; `M8_mxmem_sm_act_hf` and `M8_mxmem_wei_hf` together saturate both
; slots already. If the packetizer pulls in any subsequent instruction
; that also requires a memory slot — e.g. an `L2_loadri_io` stack
; reload for a post-HMX call argument — the MC HexagonShuffler rejects
; the resulting packet with `invalid instruction packet: slot error`.
;
; The fix forces the packetizer to *end the packet* immediately after
; emitting the act/wei pair so the next memory-bearing instruction
; starts a fresh packet. Without the fix this test fails to assemble.
;
; This shape mirrors the TIR-emitted HMX f16 matmul kernel inner block:
;   bias = mxmem2(...)
;   mxclracc.hf
;   activation.hf = mxmem(...)   \  must be in one packet
;   weight.hf     = mxmem(...)   /
;   cvt.hf = acc(R0)             \  must NOT be packetized with the
;   mxmem(...)    = cvt          /  preceding pair (slot conflict)
;
; with surrounding stack reloads that the post-RA scheduler is free to
; place between the HMX block and the drain, exercising the slot-error
; case.

declare void @llvm.hexagon.M8.mxmem2.bias(ptr)
declare void @llvm.hexagon.M8.mxclracc.hf()
declare void @llvm.hexagon.M8.mxmem.sm.act.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.hf(ptr, i32)
declare void @llvm.hexagon.M8.cvt.rs.hf(i32)
declare void @llvm.hexagon.M8.mxmem(ptr, i32)

; CHECK-LABEL: hmx_actwei_plus_stack_reload:
; The act + wei pair must end up in *one* packet (immediately
; consecutive lines with `weight.hf = mxmem` right after activation,
; then `}` closing the packet). Subsequent instructions that need
; memory slots must start a new packet.
; CHECK:        activation.hf = mxmem({{r[0-9]+}},{{r[0-9]+}}):deep
; CHECK-NEXT:   weight.hf = mxmem({{r[0-9]+}},{{r[0-9]+}})
; CHECK-NEXT: }
; The cvt.hf=acc drain is isSolo: it must be alone in its packet.
; Anchor on the cvt line, then walk backward/forward.
; CHECK:        cvt.hf = acc({{r[0-9]+}})
; CHECK-NEXT: }
; CHECK-NEXT: {
; CHECK-NEXT:   mxmem({{r[0-9]+}},{{r[0-9]+}}) = cvt
; CHECK-NEXT: }

define void @hmx_actwei_plus_stack_reload(ptr %A, ptr %B, ptr %C, ptr %bias,
                                          ptr %D, ptr %E) {
entry:
  call void @llvm.hexagon.M8.mxmem2.bias(ptr %bias)
  call void @llvm.hexagon.M8.mxclracc.hf()
  call void @llvm.hexagon.M8.mxmem.sm.act.hf(ptr %A, i32 124)
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr %B, i32 1920)
  ; Touch D and E so the scheduler keeps stack reloads alive in this
  ; region; combined with the post-RA scheduler this exercises the
  ; "scalar memory op trailing the HMX pair" packetizer path.
  %dv = load i32, ptr %D
  %ev = load i32, ptr %E
  %sum = add i32 %dv, %ev
  store i32 %sum, ptr %C
  call void @llvm.hexagon.M8.cvt.rs.hf(i32 0)
  call void @llvm.hexagon.M8.mxmem(ptr %C, i32 0)
  ret void
}
