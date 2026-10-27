; Production-shaped HMX f8 MAC tile (blk.sm.act.f8 + wei.f8 + V81 drain).
; Drain is two ops: cvt.f8=acc(Rs) then mxmem(Rs,Rt).f8=cvt.
; Rt placeholders match hf (act 0x7FFF, wei 0x780, cvt/store 0).
;
; Numerical lit is gated on hexagon-sim-hmx-f8 — Tools 19.0.04 --mhmx on
; v79/v81 QuRT currently traps on the drain; see Inputs/RECIPES.md.

target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon"

declare void @llvm.hexagon.M8.mxmem2.bias(ptr)
declare void @llvm.hexagon.M8.mxclracc()
declare void @llvm.hexagon.M8.mxmem.blk.sm.act.f8(ptr, i32)
declare void @llvm.hexagon.M8.mxmem.wei.f8(ptr, i32)
declare void @llvm.hexagon.M8.cvt.rs.f8(i32)
declare void @llvm.hexagon.M8.mxmem.f8(ptr, i32)

define void @hmx_mac_tile(ptr %bias, ptr %A, ptr %B, ptr %C) {
entry:
  call void @llvm.hexagon.M8.mxmem2.bias(ptr %bias)
  call void @llvm.hexagon.M8.mxclracc()
  call void @llvm.hexagon.M8.mxmem.blk.sm.act.f8(ptr %A, i32 32767)
  call void @llvm.hexagon.M8.mxmem.wei.f8(ptr %B, i32 1920)
  call void @llvm.hexagon.M8.cvt.rs.f8(i32 0)
  call void @llvm.hexagon.M8.mxmem.f8(ptr %C, i32 0)
  ret void
}
