; Production-shaped HMX fp16 32x32x32 MAC tile (AH x WH -> AH drain).
; Rt values match hexkl / TVM hmx_tz_*: act 0x7FFF, wei 0x780, drain 0.
;
; Compiled by in-tree llc (+hmx) so HexagonHMXPairing / packetizer are under
; test; SDK hexagon-clang is used only to link the QuRT harness.

target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon"

declare void @llvm.hexagon.M8.mxmem2.bias(ptr)
declare void @llvm.hexagon.M8.mxclracc.hf()
declare void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.hf(ptr, i32)
declare void @llvm.hexagon.M8.mxcvtr.sat.hf(ptr, i32)

define void @hmx_mac_tile(ptr %bias, ptr %A_ah, ptr %B_wh, ptr %C_ah) {
entry:
  call void @llvm.hexagon.M8.mxmem2.bias(ptr %bias)
  call void @llvm.hexagon.M8.mxclracc.hf()
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.hf(ptr %A_ah, i32 32767)
  call void @llvm.hexagon.M8.mxmem.wei.hf(ptr %B_wh, i32 1920)
  call void @llvm.hexagon.M8.mxcvtr.sat.hf(ptr %C_ah, i32 0)
  ret void
}
