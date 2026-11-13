; RUN: llc -mtriple=hexagon -mcpu=hexagonv79 -mattr=+hvxv79,+hvx-length128b,+hmx -filetype=obj %s -o /dev/null
;
; Twenty live scalars around an A16W8 HMX sequence: the input the combined
; backend fix was validated with. It must compile with assertions enabled.

; Assertions-enabled object-emission smoke from the combined backend validation.
; This checks code generation under scalar register pressure, not device numerics.
target triple = "hexagon"
declare void @llvm.hexagon.M8.mxmem2.bias(ptr)
declare void @llvm.hexagon.M8.mxclracc()
declare void @llvm.hexagon.M8.mxmem.blk.sm.act.ub(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.b(ptr, i32)
declare void @llvm.hexagon.M8.mxcvta.sat.uh(ptr, i32)
define void @integer_tile(ptr %bias, ptr %a, ptr %w, ptr %out, ptr %side) {
%p0 = getelementptr i32, ptr %side, i32 0
%v0 = load volatile i32, ptr %p0, align 4
%p1 = getelementptr i32, ptr %side, i32 1
%v1 = load volatile i32, ptr %p1, align 4
%p2 = getelementptr i32, ptr %side, i32 2
%v2 = load volatile i32, ptr %p2, align 4
%p3 = getelementptr i32, ptr %side, i32 3
%v3 = load volatile i32, ptr %p3, align 4
%p4 = getelementptr i32, ptr %side, i32 4
%v4 = load volatile i32, ptr %p4, align 4
%p5 = getelementptr i32, ptr %side, i32 5
%v5 = load volatile i32, ptr %p5, align 4
%p6 = getelementptr i32, ptr %side, i32 6
%v6 = load volatile i32, ptr %p6, align 4
%p7 = getelementptr i32, ptr %side, i32 7
%v7 = load volatile i32, ptr %p7, align 4
%p8 = getelementptr i32, ptr %side, i32 8
%v8 = load volatile i32, ptr %p8, align 4
%p9 = getelementptr i32, ptr %side, i32 9
%v9 = load volatile i32, ptr %p9, align 4
%p10 = getelementptr i32, ptr %side, i32 10
%v10 = load volatile i32, ptr %p10, align 4
%p11 = getelementptr i32, ptr %side, i32 11
%v11 = load volatile i32, ptr %p11, align 4
%p12 = getelementptr i32, ptr %side, i32 12
%v12 = load volatile i32, ptr %p12, align 4
%p13 = getelementptr i32, ptr %side, i32 13
%v13 = load volatile i32, ptr %p13, align 4
%p14 = getelementptr i32, ptr %side, i32 14
%v14 = load volatile i32, ptr %p14, align 4
%p15 = getelementptr i32, ptr %side, i32 15
%v15 = load volatile i32, ptr %p15, align 4
%p16 = getelementptr i32, ptr %side, i32 16
%v16 = load volatile i32, ptr %p16, align 4
%p17 = getelementptr i32, ptr %side, i32 17
%v17 = load volatile i32, ptr %p17, align 4
%p18 = getelementptr i32, ptr %side, i32 18
%v18 = load volatile i32, ptr %p18, align 4
%p19 = getelementptr i32, ptr %side, i32 19
%v19 = load volatile i32, ptr %p19, align 4
call void @llvm.hexagon.M8.mxclracc()
call void @llvm.hexagon.M8.mxmem.blk.sm.act.ub(ptr %a, i32 2047) nomerge
call void @llvm.hexagon.M8.mxmem.wei.b(ptr %w, i32 896) nomerge
call void @llvm.hexagon.M8.mxmem2.bias(ptr %bias)
call void @llvm.hexagon.M8.mxcvta.sat.uh(ptr %out, i32 0)
%plus0 = add i32 %v0, 1
store volatile i32 %plus0, ptr %p0, align 4
%plus1 = add i32 %v1, 1
store volatile i32 %plus1, ptr %p1, align 4
%plus2 = add i32 %v2, 1
store volatile i32 %plus2, ptr %p2, align 4
%plus3 = add i32 %v3, 1
store volatile i32 %plus3, ptr %p3, align 4
%plus4 = add i32 %v4, 1
store volatile i32 %plus4, ptr %p4, align 4
%plus5 = add i32 %v5, 1
store volatile i32 %plus5, ptr %p5, align 4
%plus6 = add i32 %v6, 1
store volatile i32 %plus6, ptr %p6, align 4
%plus7 = add i32 %v7, 1
store volatile i32 %plus7, ptr %p7, align 4
%plus8 = add i32 %v8, 1
store volatile i32 %plus8, ptr %p8, align 4
%plus9 = add i32 %v9, 1
store volatile i32 %plus9, ptr %p9, align 4
%plus10 = add i32 %v10, 1
store volatile i32 %plus10, ptr %p10, align 4
%plus11 = add i32 %v11, 1
store volatile i32 %plus11, ptr %p11, align 4
%plus12 = add i32 %v12, 1
store volatile i32 %plus12, ptr %p12, align 4
%plus13 = add i32 %v13, 1
store volatile i32 %plus13, ptr %p13, align 4
%plus14 = add i32 %v14, 1
store volatile i32 %plus14, ptr %p14, align 4
%plus15 = add i32 %v15, 1
store volatile i32 %plus15, ptr %p15, align 4
%plus16 = add i32 %v16, 1
store volatile i32 %plus16, ptr %p16, align 4
%plus17 = add i32 %v17, 1
store volatile i32 %plus17, ptr %p17, align 4
%plus18 = add i32 %v18, 1
store volatile i32 %plus18, ptr %p18, align 4
%plus19 = add i32 %v19, 1
store volatile i32 %plus19, ptr %p19, align 4
ret void
}
