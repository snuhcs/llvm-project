; Production-shaped HMX ub x wei.n MAC tile (uniform operands).
; Bias: fp16 identity scale (InsertHMXBiasSetup layout). Clear: mxclracc.
; Rt placeholders match hf production (act 0x7FFF, wei 0x780, drain 0).
; Uniform fill A=0x11 B=0x11 drains to uniform C=0x1 on hexagon-sim.

target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon"

declare void @llvm.hexagon.M8.mxmem2.bias(ptr)
declare void @llvm.hexagon.M8.mxclracc()
declare void @llvm.hexagon.M8.mxmem.blk.sm.act.ub(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.n(ptr, i32)
declare void @llvm.hexagon.M8.mxcvtr.sat.ub(ptr, i32)

define void @hmx_mac_tile(ptr %bias, ptr %A, ptr %B, ptr %C) {
entry:
  call void @llvm.hexagon.M8.mxmem2.bias(ptr %bias)
  call void @llvm.hexagon.M8.mxclracc()
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.ub(ptr %A, i32 32767)
  call void @llvm.hexagon.M8.mxmem.wei.n(ptr %B, i32 1920)
  call void @llvm.hexagon.M8.mxcvtr.sat.ub(ptr %C, i32 0)
  ret void
}
