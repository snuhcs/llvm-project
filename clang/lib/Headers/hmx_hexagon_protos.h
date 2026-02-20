//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//

#ifndef _HMX_HEXAGON_PROTOS_H_
#define _HMX_HEXAGON_PROTOS_H_ 1

#ifdef __HMX__
/* ==========================================================================
   Assembly Syntax:       acc=mxshl(acc,#16)
   C Intrinsic Prototype: void Q6_acc_mxshl_acc()
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_acc_mxshl_acc __builtin_HEXAGON_M8_mxaccshl

/* ==========================================================================
   Assembly Syntax:       mxclracc
   C Intrinsic Prototype: void Q6_mxclracc()
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxclracc __builtin_HEXAGON_M8_mxclracc

/* ==========================================================================
   Assembly Syntax:       mxclracc.hf
   C Intrinsic Prototype: void Q6_mxclracc_hf()
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxclracc_hf __builtin_HEXAGON_M8_mxclracc_hf

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:sat.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_after_sat_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_sat_uh_2x1 __builtin_HEXAGON_M8_mxcvta_sat_uh

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain:sat.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_sat_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_sat_uh_2x1 __builtin_HEXAGON_M8_mxcvta_sat_uh_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_after_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_uh_2x1 __builtin_HEXAGON_M8_mxcvta_uh

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_uh_2x1 __builtin_HEXAGON_M8_mxcvta_uh_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:sat.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_before_sat_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_sat_uh_2x1 __builtin_HEXAGON_M8_mxcvtb_sat_uh

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain:sat.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_sat_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_sat_uh_2x1 __builtin_HEXAGON_M8_mxcvtb_sat_uh_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_before_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_uh_2x1 __builtin_HEXAGON_M8_mxcvtb_uh

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain.uh=acc:2x1
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_uh_2x1(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_uh_2x1 __builtin_HEXAGON_M8_mxcvtb_uh_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:cm:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_cm_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_cm_sat_ub __builtin_HEXAGON_M8_mxcvtl_dm_sat_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain:cm:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_cm_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_cm_sat_ub __builtin_HEXAGON_M8_mxcvtl_dm_sat_ub_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:cm.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_cm_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_cm_ub __builtin_HEXAGON_M8_mxcvtl_dm_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain:cm.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_cm_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_cm_ub __builtin_HEXAGON_M8_mxcvtl_dm_ub_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_hf __builtin_HEXAGON_M8_mxcvtl_sat_hf

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_hf __builtin_HEXAGON_M8_mxcvtl_sat_hf_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:pos.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_pos_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_pos_hf __builtin_HEXAGON_M8_mxcvtl_sat_pos_hf

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain:pos.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_pos_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_pos_hf __builtin_HEXAGON_M8_mxcvtl_sat_pos_hf_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_sat_ub __builtin_HEXAGON_M8_mxcvtl_sat_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_sat_ub __builtin_HEXAGON_M8_mxcvtl_sat_ub_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_ub __builtin_HEXAGON_M8_mxcvtl_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_ub __builtin_HEXAGON_M8_mxcvtl_ub_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:cm:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_cm_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_cm_sat_ub __builtin_HEXAGON_M8_mxcvtr_dm_sat_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain:cm:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_cm_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_cm_sat_ub __builtin_HEXAGON_M8_mxcvtr_dm_sat_ub_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:cm.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_cm_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_cm_ub __builtin_HEXAGON_M8_mxcvtr_dm_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain:cm.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_cm_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_cm_ub __builtin_HEXAGON_M8_mxcvtr_dm_ub_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_hf __builtin_HEXAGON_M8_mxcvtr_sat_hf

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_hf __builtin_HEXAGON_M8_mxcvtr_sat_hf_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:pos.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_pos_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_pos_hf __builtin_HEXAGON_M8_mxcvtr_sat_pos_hf

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain:pos.hf=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_pos_hf(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_pos_hf __builtin_HEXAGON_M8_mxcvtr_sat_pos_hf_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_sat_ub __builtin_HEXAGON_M8_mxcvtr_sat_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain:sat.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_sat_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_sat_ub __builtin_HEXAGON_M8_mxcvtr_sat_ub_r

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_ub __builtin_HEXAGON_M8_mxcvtr_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain.ub=acc
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_ub(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_ub __builtin_HEXAGON_M8_mxcvtr_ub_r

/* ==========================================================================
   Assembly Syntax:       bias=mxmem2(Rs32)
   C Intrinsic Prototype: void Q6_bias_mxmem2_A(Address Rs)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_bias_mxmem2_A __builtin_HEXAGON_M8_mxmem2_bias

/* ==========================================================================
   Assembly Syntax:       mxmem2(Rs32)=bias
   C Intrinsic Prototype: void Q6_mxmem2_bias_A(Address Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem2_bias_A __builtin_HEXAGON_M8_mxmem2_st_bias

/* ==========================================================================
   Assembly Syntax:       bias=mxmem(Rs32)
   C Intrinsic Prototype: void Q6_bias_mxmem_A(Address Rs)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_bias_mxmem_A __builtin_HEXAGON_M8_mxmem_bias

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):cm
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_cm(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_cm __builtin_HEXAGON_M8_mxmem_blk_dm_act_ub

/* ==========================================================================
   Assembly Syntax:       activation.hf=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_activation_hf_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_hf_mxmem_RR __builtin_HEXAGON_M8_mxmem_blk_sm_act_hf

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR __builtin_HEXAGON_M8_mxmem_blk_sm_act_ub

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):deep:cm
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_deep_cm(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_deep_cm __builtin_HEXAGON_M8_mxmem_dm_act_ub

/* ==========================================================================
   Assembly Syntax:       activation.hf=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_activation_hf_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_hf_mxmem_RR_deep __builtin_HEXAGON_M8_mxmem_sm_act_hf

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_deep __builtin_HEXAGON_M8_mxmem_sm_act_ub

/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32)=bias
   C Intrinsic Prototype: void Q6_mxmem_bias_A(Address Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_bias_A __builtin_HEXAGON_M8_mxmem_st_bias

/* ==========================================================================
   Assembly Syntax:       weight.b=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_b_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_b_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_b

/* ==========================================================================
   Assembly Syntax:       weight.ubit=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_ubit_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_ubit_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_b1

/* ==========================================================================
   Assembly Syntax:       weight.c=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_c_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_c_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_c

/* ==========================================================================
   Assembly Syntax:       weight.hf=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_hf_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_hf_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_hf

/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_n

/* ==========================================================================
   Assembly Syntax:       weight.sbit=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_sbit_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sbit_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_sb1

/* ==========================================================================
   Assembly Syntax:       weight.sc=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_sc_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sc_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_sc

/* ==========================================================================
   Assembly Syntax:       weight.sm=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_sm_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sm_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_sm

/* ==========================================================================
   Assembly Syntax:       weight.b=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_b_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_b_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_b

/* ==========================================================================
   Assembly Syntax:       weight.ubit=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_ubit_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_ubit_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_b1

/* ==========================================================================
   Assembly Syntax:       weight.c=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_c_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_c_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_c

/* ==========================================================================
   Assembly Syntax:       weight.hf=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_hf_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_hf_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_hf

/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_n

/* ==========================================================================
   Assembly Syntax:       weight.sbit=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_sbit_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sbit_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_sb1

/* ==========================================================================
   Assembly Syntax:       weight.sc=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_sc_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sc_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_sc

/* ==========================================================================
   Assembly Syntax:       weight.sm=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_sm_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sm_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_sm

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):dilate:cm
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_dilate_cm(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_dilate_cm __builtin_HEXAGON_M8_mxmemd_blk_dm_act_ub

/* ==========================================================================
   Assembly Syntax:       activation.hf=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_activation_hf_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_hf_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemd_blk_sm_act_hf

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemd_blk_sm_act_ub

/* ==========================================================================
   Assembly Syntax:       weight.b=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_b_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_b_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_b

/* ==========================================================================
   Assembly Syntax:       weight.ubit=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_ubit_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_ubit_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_b1

/* ==========================================================================
   Assembly Syntax:       weight.c=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_c_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_c_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_c

/* ==========================================================================
   Assembly Syntax:       weight.hf=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_hf_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_hf_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_hf

/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_n

/* ==========================================================================
   Assembly Syntax:       weight.sbit=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_sbit_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sbit_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_sb1

/* ==========================================================================
   Assembly Syntax:       weight.sc=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_sc_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sc_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_sc

/* ==========================================================================
   Assembly Syntax:       weight.sm=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_sm_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sm_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_sm

/* ==========================================================================
   Assembly Syntax:       weight.b=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_b_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_b_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_b

/* ==========================================================================
   Assembly Syntax:       weight.ubit=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_ubit_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_ubit_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_b1

/* ==========================================================================
   Assembly Syntax:       weight.c=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_c_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_c_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_c

/* ==========================================================================
   Assembly Syntax:       weight.hf=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_hf_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_hf_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_hf

/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_n

/* ==========================================================================
   Assembly Syntax:       weight.sbit=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_sbit_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sbit_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_sb1

/* ==========================================================================
   Assembly Syntax:       weight.sc=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_sc_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sc_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_sc

/* ==========================================================================
   Assembly Syntax:       weight.sm=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_sm_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sm_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_sm

/* ==========================================================================
   Assembly Syntax:       weight.b=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_b_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_b_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_b

/* ==========================================================================
   Assembly Syntax:       weight.ubit=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_ubit_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_ubit_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_b1

/* ==========================================================================
   Assembly Syntax:       weight.c=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_c_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_c_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_c

/* ==========================================================================
   Assembly Syntax:       weight.hf=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_hf_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_hf_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_hf

/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_n

/* ==========================================================================
   Assembly Syntax:       weight.sbit=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_sbit_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sbit_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_sb1

/* ==========================================================================
   Assembly Syntax:       weight.sc=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_sc_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sc_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_sc

/* ==========================================================================
   Assembly Syntax:       weight.sm=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_sm_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sm_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_sm

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):single:cm
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_single_cm(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_single_cm __builtin_HEXAGON_M8_mxmems_blk_dm_act_ub

/* ==========================================================================
   Assembly Syntax:       activation.hf=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_activation_hf_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_hf_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_blk_sm_act_hf

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_blk_sm_act_ub

/* ==========================================================================
   Assembly Syntax:       weight.b=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_b_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_b_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_b

/* ==========================================================================
   Assembly Syntax:       weight.ubit=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_ubit_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_ubit_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_b1

/* ==========================================================================
   Assembly Syntax:       weight.c=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_c_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_c_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_c

/* ==========================================================================
   Assembly Syntax:       weight.hf=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_hf_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_hf_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_hf

/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_n

/* ==========================================================================
   Assembly Syntax:       weight.sbit=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_sbit_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sbit_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_sb1

/* ==========================================================================
   Assembly Syntax:       weight.sc=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_sc_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sc_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_sc

/* ==========================================================================
   Assembly Syntax:       weight.sm=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_sm_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_sm_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_sm

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):above:cm
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_above_cm(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_above_cm __builtin_HEXAGON_M8_mxmemu_blk_dm_act_ub

/* ==========================================================================
   Assembly Syntax:       activation.hf=mxmem(Rs32,Rt32):above
   C Intrinsic Prototype: void Q6_activation_hf_mxmem_RR_above(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_hf_mxmem_RR_above __builtin_HEXAGON_M8_mxmemu_blk_sm_act_hf

/* ==========================================================================
   Assembly Syntax:       activation.ub=mxmem(Rs32,Rt32):above
   C Intrinsic Prototype: void Q6_activation_ub_mxmem_RR_above(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_ub_mxmem_RR_above __builtin_HEXAGON_M8_mxmemu_blk_sm_act_ub

/* ==========================================================================
   Assembly Syntax:       mxswapacc
   C Intrinsic Prototype: void Q6_mxswapacc()
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxswapacc __builtin_HEXAGON_M8_mxswap

/* ==========================================================================
   Assembly Syntax:       mxswapacc.hf
   C Intrinsic Prototype: void Q6_mxswapacc_hf()
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxswapacc_hf __builtin_HEXAGON_M8_mxswap_hf

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:sat.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_after_sat_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_sat_uh_2x2 __builtin_HEXAGON_M8_mxcvta_sat_uh2x2
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain:sat.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_sat_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_sat_uh_2x2 __builtin_HEXAGON_M8_mxcvta_sat_uh2x2_r
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_after_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_uh_2x2 __builtin_HEXAGON_M8_mxcvta_uh2x2
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):after:retain.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_after_retain_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_after_retain_uh_2x2 __builtin_HEXAGON_M8_mxcvta_uh2x2_r
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:sat.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_before_sat_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_sat_uh_2x2 __builtin_HEXAGON_M8_mxcvtb_sat_uh2x2
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain:sat.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_sat_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_sat_uh_2x2 __builtin_HEXAGON_M8_mxcvtb_sat_uh2x2_r
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_before_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_uh_2x2 __builtin_HEXAGON_M8_mxcvtb_uh2x2
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 69
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):before:retain.uh=acc:2x2
   C Intrinsic Prototype: void Q6_mxmem_AR_before_retain_uh_2x2(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_AR_before_retain_uh_2x2 __builtin_HEXAGON_M8_mxcvtb_uh2x2_r
#endif /* __HEXAGON_ARCH___ >= 69 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       cvt.hf=acc(Rs32)
   C Intrinsic Prototype: void Q6_cvt_hf_acc_R(Word32 Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_cvt_hf_acc_R __builtin_HEXAGON_M8_cvt_rs_hf
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       cvt.ub=acc(Rs32)
   C Intrinsic Prototype: void Q6_cvt_ub_acc_R(Word32 Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_cvt_ub_acc_R __builtin_HEXAGON_M8_cvt_rs_ub
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       cvt.ub=acc(Rs32):sc0
   C Intrinsic Prototype: void Q6_cvt_ub_acc_R_sc0(Word32 Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_cvt_ub_acc_R_sc0 __builtin_HEXAGON_M8_cvt_rs_ub_sc0
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       cvt.ub=acc(Rs32):sc1
   C Intrinsic Prototype: void Q6_cvt_ub_acc_R_sc1(Word32 Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_cvt_ub_acc_R_sc1 __builtin_HEXAGON_M8_cvt_rs_ub_sc1
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       cvt.uh=acc(Rs32):2x1
   C Intrinsic Prototype: void Q6_cvt_uh_acc_R_2x1(Word32 Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_cvt_uh_acc_R_2x1 __builtin_HEXAGON_M8_cvt_rs_uh_2x1
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       cvt.uh=acc(Rs32):2x2
   C Intrinsic Prototype: void Q6_cvt_uh_acc_R_2x2(Word32 Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_cvt_uh_acc_R_2x2 __builtin_HEXAGON_M8_cvt_rs_uh_2x2
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32)=cvt
   C Intrinsic Prototype: void Q6_mxmem_cvt_AR(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_cvt_AR __builtin_HEXAGON_M8_mxmem
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):2x2=cvt
   C Intrinsic Prototype: void Q6_mxmem_cvt_AR_2x2=cvt(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_cvt_AR_2x2=cvt __builtin_HEXAGON_M8_mxmem_2x2
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):cm=cvt
   C Intrinsic Prototype: void Q6_mxmem_cvt_AR_cm=cvt(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_cvt_AR_cm=cvt __builtin_HEXAGON_M8_mxmem_cm
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):2x
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_2x(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_2x __builtin_HEXAGON_M8_mxmem_wei_n_2x
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):2x:after
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_2x_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_2x_after __builtin_HEXAGON_M8_mxmema_wei_n_2x
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):2x:dilate
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_2x_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_2x_dilate __builtin_HEXAGON_M8_mxmemdi_wei_n_2x
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):2x:deep
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_2x_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_2x_deep __builtin_HEXAGON_M8_mxmemdp_wei_n_2x
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):2x:drop
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_2x_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_2x_drop __builtin_HEXAGON_M8_mxmemdr_wei_n_2x
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 73
/* ==========================================================================
   Assembly Syntax:       weight.n=mxmem(Rs32,Rt32):2x:single
   C Intrinsic Prototype: void Q6_weight_n_mxmem_RR_2x_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_n_mxmem_RR_2x_single __builtin_HEXAGON_M8_mxmems_wei_n_2x
#endif /* __HEXAGON_ARCH___ >= 73 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       activation.f8=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_activation_f8_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_f8_mxmem_RR __builtin_HEXAGON_M8_mxmem_blk_sm_act_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):cm:deep=cvt
   C Intrinsic Prototype: void Q6_mxmem_cvt_AR_cm_deep=cvt(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_cvt_AR_cm_deep=cvt __builtin_HEXAGON_M8_mxmem_cm_deep
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):deep=cvt
   C Intrinsic Prototype: void Q6_mxmem_cvt_AR_deep=cvt(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_cvt_AR_deep=cvt __builtin_HEXAGON_M8_mxmem_deep
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       activation.f8=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_activation_f8_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_f8_mxmem_RR_deep __builtin_HEXAGON_M8_mxmem_sm_act_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       weight.f8=mxmem(Rs32,Rt32)
   C Intrinsic Prototype: void Q6_weight_f8_mxmem_RR(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_f8_mxmem_RR __builtin_HEXAGON_M8_mxmem_wei_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       weight.f8=mxmem(Rs32,Rt32):after
   C Intrinsic Prototype: void Q6_weight_f8_mxmem_RR_after(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_f8_mxmem_RR_after __builtin_HEXAGON_M8_mxmema_wei_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       activation.f8=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_activation_f8_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_f8_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemd_blk_sm_act_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       weight.f8=mxmem(Rs32,Rt32):dilate
   C Intrinsic Prototype: void Q6_weight_f8_mxmem_RR_dilate(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_f8_mxmem_RR_dilate __builtin_HEXAGON_M8_mxmemdi_wei_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       weight.f8=mxmem(Rs32,Rt32):deep
   C Intrinsic Prototype: void Q6_weight_f8_mxmem_RR_deep(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_f8_mxmem_RR_deep __builtin_HEXAGON_M8_mxmemdp_wei_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       weight.f8=mxmem(Rs32,Rt32):drop
   C Intrinsic Prototype: void Q6_weight_f8_mxmem_RR_drop(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_f8_mxmem_RR_drop __builtin_HEXAGON_M8_mxmemdr_wei_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       activation.f8=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_activation_f8_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_f8_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_blk_sm_act_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       weight.f8=mxmem(Rs32,Rt32):single
   C Intrinsic Prototype: void Q6_weight_f8_mxmem_RR_single(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_weight_f8_mxmem_RR_single __builtin_HEXAGON_M8_mxmems_wei_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 79
/* ==========================================================================
   Assembly Syntax:       activation.f8=mxmem(Rs32,Rt32):above
   C Intrinsic Prototype: void Q6_activation_f8_mxmem_RR_above(UWord32 Rs, Word32 Rt)
   Instruction Type:      LD
   Execution Slots:       SLOT01
   ========================================================================== */

#define Q6_activation_f8_mxmem_RR_above __builtin_HEXAGON_M8_mxmemu_blk_sm_act_f8
#endif /* __HEXAGON_ARCH___ >= 79 */

#if __HMX_ARCH__ >= 81
/* ==========================================================================
   Assembly Syntax:       cvt.f8=acc(Rs32)
   C Intrinsic Prototype: void Q6_cvt_f8_acc_R(Word32 Rs)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_cvt_f8_acc_R __builtin_HEXAGON_M8_cvt_rs_f8
#endif /* __HEXAGON_ARCH___ >= 81 */

#if __HMX_ARCH__ >= 81
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32):deep.f8=cvt
   C Intrinsic Prototype: void Q6_mxmem_cvt_AR_deep_f8=cvt(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_cvt_AR_deep_f8=cvt __builtin_HEXAGON_M8_mxmem_deep_f8
#endif /* __HEXAGON_ARCH___ >= 81 */

#if __HMX_ARCH__ >= 81
/* ==========================================================================
   Assembly Syntax:       mxmem(Rs32,Rt32).f8=cvt
   C Intrinsic Prototype: void Q6_mxmem_cvt_AR(Address Rs, Word32 Rt)
   Instruction Type:      ST
   Execution Slots:       SLOT0
   ========================================================================== */

#define Q6_mxmem_cvt_AR __builtin_HEXAGON_M8_mxmem_f8
#endif /* __HEXAGON_ARCH___ >= 81 */

#endif /* __HMX__ */

#endif
