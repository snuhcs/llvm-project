; RUN: llc -mtriple=hexagon -mcpu=hexagonv75 -mattr=+hvxv75,+hvx-length128b,+hmx < %s | FileCheck %s
;
; X-D: SSA PS_hmx_actwei keeps act+wei one MI through RA, so the k-loop
; latch cannot land between them. The trip compare must not read the HMX
; Rt immediate (always false), or packed-B DMA src walks off B_global.
;
; Shape mirrors TVM linear HMX `main_compute_` call_end91: blk-sm act
; (Rt=32767), wei (Rt=1920), trip compare, loop-carried pointer bump, and
; an opaque call so post-RA can place scalar ops in the act-wei window.

declare void @llvm.hexagon.M8.mxclracc.hf()
declare void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.hf(ptr, i32)
declare void @sink(i32)

; CHECK-LABEL: hmx_k_loop_latch:
; Act + wei stay adjacent in one packet (packetizer requirement).
; CHECK:        activation.hf = mxmem({{r[0-9]+}},[[RT:r[0-9]+]])
; CHECK-NEXT:   weight.hf = mxmem({{r[0-9]+}},{{r[0-9]+}})
; CHECK-NEXT: }
;
; The physreg that is act Rt must not be the trip-compare operand
; (`cmp.eq(r1,##7168)` after `r1 = #32767` was the X-D failure).
; CHECK-NOT: cmp.eq([[RT]],##7168)
; CHECK-NOT: cmp.eq([[RT]],#7)

define void @hmx_k_loop_latch(ptr %a, ptr %w) {
entry:
  br label %loop

loop:
  %i = phi i32 [ 0, %entry ], [ %inext, %loop ]
  %ap = getelementptr i8, ptr %a, i32 %i
  %wp = getelementptr i8, ptr %w, i32 %i
  call void @llvm.hexagon.M8.mxclracc.hf()
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr %ap, i32 32767)
  call void @sink(i32 %i)
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr %wp, i32 1920)
  %inext = add nuw nsw i32 %i, 1024
  %c = icmp eq i32 %inext, 7168
  br i1 %c, label %exit, label %loop

exit:
  ret void
}
