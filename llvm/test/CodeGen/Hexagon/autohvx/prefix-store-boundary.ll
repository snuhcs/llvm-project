; RUN: llc -mtriple=hexagon -mcpu=hexagonv79 -mattr=+hvxv79,+hvx-length128b -hexagon-hvx-widen=16 < %s | FileCheck %s --check-prefix=HVX128
; RUN: llc -mtriple=hexagon -mcpu=hexagonv65 -mattr=+hvxv65,+hvx-length64b -hexagon-hvx-widen=16 < %s | FileCheck %s --check-prefix=HVX64

; The split upper store must address the vector containing the last enabled
; byte. When dst+64 is a page boundary, the upper mask is empty: addressing
; dst+128 can still fault. Using dst+63 also handles a genuinely crossing store.
; HVX128-LABEL: copy64:
; HVX128: [[END:r[0-9]+]] = add(r0,#63)
; HVX128: if (q{{[0-3]}}) vmem([[END]]+#0) = v{{[0-9]+}}
; HVX128-NOT: vmem(r0+#1)
define void @copy64(ptr %dst, ptr %src) {
  %v = load <32 x half>, ptr %src, align 2
  store <32 x half> %v, ptr %dst, align 2
  ret void
}

; HVX128-LABEL: copy32:
; HVX128: [[END:r[0-9]+]] = add(r0,#31)
; HVX128: if (q{{[0-3]}}) vmem([[END]]+#0) = v{{[0-9]+}}
; HVX128-NOT: vmem(r0+#1)
; HVX64-LABEL: copy32:
; HVX64: [[END:r[0-9]+]] = add(r0,#31)
; HVX64: if (q{{[0-3]}}) vmem([[END]]+#0) = v{{[0-9]+}}
; HVX64-NOT: vmem(r0+#1)
; HVX64: .Lfunc_end1:
define void @copy32(ptr %dst, ptr %src) {
  %v = load <32 x i8>, ptr %src, align 1
  store <32 x i8> %v, ptr %dst, align 1
  ret void
}

; Aligned stores need no second address.
; HVX128-LABEL: aligned64:
; HVX128-NOT: add(r0,
; HVX128: if (q{{[0-3]}}) vmem(r0+#0) = v{{[0-9]+}}
define void @aligned64(ptr %dst, ptr %src) {
  %v = load <32 x half>, ptr %src, align 128
  store <32 x half> %v, ptr %dst, align 128
  ret void
}

; An all-zero mask must emit no stores, including no access to a null pointer.
; HVX128-LABEL: empty:
; HVX128-NOT: vmem
; HVX128: jumpr r31
define void @empty() {
  call void @llvm.masked.store.v128i8.p0(<128 x i8> zeroinitializer, ptr null, i32 1, <128 x i1> zeroinitializer)
  ret void
}

; Arbitrary masks retain both parts without assuming a prefix extent.
; HVX128-LABEL: dynamic:
; HVX128: if (q{{[0-3]}}) vmem(r0+#1) = v{{[0-9]+}}
; HVX128: if (q{{[0-3]}}) vmem(r0+#0) = v{{[0-9]+}}
define void @dynamic(ptr %dst, ptr %src) {
  %v = load <128 x i8>, ptr %src, align 128
  %mask = icmp ne <128 x i8> %v, zeroinitializer
  call void @llvm.masked.store.v128i8.p0(<128 x i8> %v, ptr %dst, i32 1, <128 x i1> %mask)
  ret void
}

declare void @llvm.masked.store.v128i8.p0(<128 x i8>, ptr, i32 immarg, <128 x i1>)
