; RUN: llc -mtriple=hexagon -mcpu=hexagonv79 -mattr=+hvxv79,+hvx-length128b,+hvx-qfloat,+hmx -O3 < %s | FileCheck %s
; RUN: llc -mtriple=hexagon -mcpu=hexagonv79 -mattr=+hvxv79,+hvx-length128b,+hvx-qfloat,+hmx -O3 < %s \
; RUN:   | grep -A1 'activation.hf = mxmem' | not grep '}'
;
; A loop of HMX activation/weight pairs whose trip count is known only at run
; time, as the IR optimizer leaves it: a main loop unrolled by 8 and a remainder
; loop. MachinePipeliner runs before the pre-RA glue (HexagonMergeActivateWeight)
; and schedules an act and its wei as two instructions, so it can put them in
; different stages of the pipelined loop. The glue pairs act and wei only
; within one block, and the packetizer then stopped the build with
; "HMX activation mxmem must be immediately followed by weight mxmem in the
; same block". HexagonInstrInfo::analyzeLoopForPipelining now declines a loop
; that holds an HMX act or wei, and every pair below stays in one packet (the
; second RUN line: no packet closes right after an activation).
;
; Reduced from a TVM-built kernel (one accumulator, a run-time number of K
; tiles, one drain).

; CHECK-LABEL: hmx_k_loop_runtime_trip:
; CHECK:      activation.hf = mxmem
; CHECK-NEXT: weight.hf = mxmem
; CHECK:      :after.hf = acc

define void @hmx_k_loop_runtime_trip(ptr noalias %av, ptr noalias %bv, ptr noalias %cv, i32 %n) #0 {
entry:
  call void @llvm.hexagon.M8.mxclracc.hf()
  %pos = icmp sgt i32 %n, 0
  br i1 %pos, label %pre, label %done

pre:
  %xtra = and i32 %n, 7
  %small = icmp ult i32 %n, 8
  br i1 %small, label %main.exit, label %main.pre

main.pre:
  %unroll = and i32 %n, 2147483640
  br label %main

main:
  %k = phi i32 [ 0, %main.pre ], [ %k.next, %main ]
  %it = phi i32 [ 0, %main.pre ], [ %it.next, %main ]
  %s0 = shl i32 %k, 10
  %a0 = getelementptr inbounds nuw half, ptr %av, i32 %s0
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a0, i32 2047)
  %b0 = getelementptr inbounds nuw half, ptr %bv, i32 %s0
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b0, i32 1920)
  %s10 = shl i32 %k, 10
  %s1 = or disjoint i32 %s10, 1024
  %a1 = getelementptr inbounds nuw half, ptr %av, i32 %s1
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a1, i32 2047)
  %b1 = getelementptr inbounds nuw half, ptr %bv, i32 %s1
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b1, i32 1920)
  %s20 = shl i32 %k, 10
  %s2 = or disjoint i32 %s20, 2048
  %a2 = getelementptr inbounds nuw half, ptr %av, i32 %s2
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a2, i32 2047)
  %b2 = getelementptr inbounds nuw half, ptr %bv, i32 %s2
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b2, i32 1920)
  %s30 = shl i32 %k, 10
  %s3 = or disjoint i32 %s30, 3072
  %a3 = getelementptr inbounds nuw half, ptr %av, i32 %s3
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a3, i32 2047)
  %b3 = getelementptr inbounds nuw half, ptr %bv, i32 %s3
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b3, i32 1920)
  %s40 = shl i32 %k, 10
  %s4 = or disjoint i32 %s40, 4096
  %a4 = getelementptr inbounds nuw half, ptr %av, i32 %s4
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a4, i32 2047)
  %b4 = getelementptr inbounds nuw half, ptr %bv, i32 %s4
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b4, i32 1920)
  %s50 = shl i32 %k, 10
  %s5 = or disjoint i32 %s50, 5120
  %a5 = getelementptr inbounds nuw half, ptr %av, i32 %s5
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a5, i32 2047)
  %b5 = getelementptr inbounds nuw half, ptr %bv, i32 %s5
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b5, i32 1920)
  %s60 = shl i32 %k, 10
  %s6 = or disjoint i32 %s60, 6144
  %a6 = getelementptr inbounds nuw half, ptr %av, i32 %s6
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a6, i32 2047)
  %b6 = getelementptr inbounds nuw half, ptr %bv, i32 %s6
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b6, i32 1920)
  %s70 = shl i32 %k, 10
  %s7 = or disjoint i32 %s70, 7168
  %a7 = getelementptr inbounds nuw half, ptr %av, i32 %s7
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %a7, i32 2047)
  %b7 = getelementptr inbounds nuw half, ptr %bv, i32 %s7
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %b7, i32 1920)
  %k.next = add nuw nsw i32 %k, 8
  %it.next = add i32 %it, 8
  %main.done = icmp eq i32 %it.next, %unroll
  br i1 %main.done, label %main.exit, label %main

main.exit:
  %k.unr = phi i32 [ 0, %pre ], [ %k.next, %main ]
  %no.rem = icmp eq i32 %xtra, 0
  br i1 %no.rem, label %done, label %rem

rem:
  %kr = phi i32 [ %kr.next, %rem ], [ %k.unr, %main.exit ]
  %ri = phi i32 [ %ri.next, %rem ], [ 0, %main.exit ]
  %sr = shl nsw i32 %kr, 10
  %ar = getelementptr inbounds nuw half, ptr %av, i32 %sr
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr nonnull %ar, i32 2047)
  %br = getelementptr inbounds nuw half, ptr %bv, i32 %sr
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr nonnull %br, i32 1920)
  %kr.next = add nuw nsw i32 %kr, 1
  %ri.next = add i32 %ri, 1
  %rem.done = icmp eq i32 %ri.next, %xtra
  br i1 %rem.done, label %done, label %rem

done:
  call void @llvm.hexagon.M8.mxcvtr.sat.hf(ptr nonnull %cv, i32 0)
  ret void
}

declare void @llvm.hexagon.M8.mxclracc.hf()
declare void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxcvtr.sat.hf(ptr, i32)

attributes #0 = { nounwind "target-cpu"="hexagonv79" "target-features"="+hvxv79,+hvx-length128b,+hvx-qfloat,+hmx" }
