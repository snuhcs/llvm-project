; RUN: llc -mtriple=hexagon -mcpu=hexagonv79 -mattr=+hvxv79,+hvx-length128b,+hvx-qfloat < %s | FileCheck %s --check-prefix=HVX128
; RUN: llc -mtriple=hexagon -mcpu=hexagonv68 -mattr=+hvxv68,+hvx-length64b,+hvx-qfloat < %s | FileCheck %s --check-prefix=HVX64

; A QF16 intermediate loses valid subnormal half products. For example,
; half(0x8332) * half(0x3533) must produce half(0x810a), not negative zero.
; Keep the product in QF32 until the final half rounding.

; HVX128-LABEL: mul64:
; HVX128-NOT: .qf16
; HVX128: v[[HI:[0-9]+]]:[[LO:[0-9]+]].qf32 = vmpy(v{{[0-9]+}}.hf,v{{[0-9]+}}.hf)
; HVX128: v{{[0-9]+}}.hf = v[[HI]]:[[LO]].qf32
; HVX128-NOT: .qf16
; HVX128: .Lfunc_end0:
define void @mul64(ptr %dst, ptr %lhs, ptr %rhs) {
  %a = load <64 x half>, ptr %lhs, align 128
  %b = load <64 x half>, ptr %rhs, align 128
  %product = fmul <64 x half> %a, %b
  store <64 x half> %product, ptr %dst, align 128
  ret void
}

; HVX64-LABEL: mul32:
; HVX64-NOT: .qf16
; HVX64: call __extendhfsf2
; HVX64: sfmpy
; HVX64: call __truncsfhf2
; HVX64-NOT: .qf16
; HVX64: .Lfunc_end1:
define void @mul32(ptr %dst, ptr %lhs, ptr %rhs) {
  %a = load <32 x half>, ptr %lhs, align 64
  %b = load <32 x half>, ptr %rhs, align 64
  %product = fmul <32 x half> %a, %b
  store <32 x half> %product, ptr %dst, align 64
  ret void
}
