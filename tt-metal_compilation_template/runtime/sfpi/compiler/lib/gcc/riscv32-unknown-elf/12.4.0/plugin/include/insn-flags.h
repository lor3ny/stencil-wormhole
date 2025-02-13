/* Generated automatically by the program `genflags'
   from the machine description file `md'.  */

#ifndef GCC_INSN_FLAGS_H
#define GCC_INSN_FLAGS_H

#define HAVE_addsf3 (TARGET_HARD_FLOAT)
#define HAVE_adddf3 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_addsi3 1
#define HAVE_adddi3 (TARGET_64BIT)
#define HAVE_subsf3 (TARGET_HARD_FLOAT)
#define HAVE_subdf3 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_subdi3 (TARGET_64BIT)
#define HAVE_subsi3 1
#define HAVE_negdi2 (TARGET_64BIT)
#define HAVE_negsi2 1
#define HAVE_mulsf3 (TARGET_HARD_FLOAT)
#define HAVE_muldf3 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_mulsi3 (TARGET_MUL)
#define HAVE_muldi3 (TARGET_MUL && TARGET_64BIT)
#define HAVE_smuldi3_highpart (TARGET_MUL && TARGET_64BIT)
#define HAVE_umuldi3_highpart (TARGET_MUL && TARGET_64BIT)
#define HAVE_usmuldi3_highpart (TARGET_MUL && TARGET_64BIT)
#define HAVE_smulsi3_highpart (TARGET_MUL && !TARGET_64BIT)
#define HAVE_umulsi3_highpart (TARGET_MUL && !TARGET_64BIT)
#define HAVE_usmulsi3_highpart (TARGET_MUL && !TARGET_64BIT)
#define HAVE_divsi3 (TARGET_DIV)
#define HAVE_udivsi3 (TARGET_DIV)
#define HAVE_modsi3 (TARGET_DIV)
#define HAVE_umodsi3 (TARGET_DIV)
#define HAVE_divdi3 (TARGET_DIV && TARGET_64BIT)
#define HAVE_udivdi3 (TARGET_DIV && TARGET_64BIT)
#define HAVE_moddi3 (TARGET_DIV && TARGET_64BIT)
#define HAVE_umoddi3 (TARGET_DIV && TARGET_64BIT)
#define HAVE_divsf3 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_HARD_FLOAT))
#define HAVE_divdf3 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_DOUBLE_FLOAT))
#define HAVE_sqrtsf2 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_HARD_FLOAT))
#define HAVE_sqrtdf2 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_DOUBLE_FLOAT))
#define HAVE_fmasf4 (TARGET_HARD_FLOAT)
#define HAVE_fmadf4 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_fmssf4 (TARGET_HARD_FLOAT)
#define HAVE_fmsdf4 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_fnmssf4 (TARGET_HARD_FLOAT)
#define HAVE_fnmsdf4 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_fnmasf4 (TARGET_HARD_FLOAT)
#define HAVE_fnmadf4 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_abssf2 (TARGET_HARD_FLOAT)
#define HAVE_absdf2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_copysignsf3 (TARGET_HARD_FLOAT)
#define HAVE_copysigndf3 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_negsf2 (TARGET_HARD_FLOAT)
#define HAVE_negdf2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_sminsf3 (TARGET_HARD_FLOAT)
#define HAVE_smindf3 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_smaxsf3 (TARGET_HARD_FLOAT)
#define HAVE_smaxdf3 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_andsi3 (!TARGET_64BIT)
#define HAVE_iorsi3 (!TARGET_64BIT)
#define HAVE_xorsi3 (!TARGET_64BIT)
#define HAVE_anddi3 (TARGET_64BIT)
#define HAVE_iordi3 (TARGET_64BIT)
#define HAVE_xordi3 (TARGET_64BIT)
#define HAVE_one_cmplsi2 (!TARGET_64BIT)
#define HAVE_one_cmpldi2 (TARGET_64BIT)
#define HAVE_truncdfsf2 (TARGET_DOUBLE_FLOAT)
#define HAVE_zero_extendqihi2 1
#define HAVE_zero_extendqisi2 1
#define HAVE_zero_extendqidi2 (TARGET_64BIT)
#define HAVE_extendsidi2 (TARGET_64BIT)
#define HAVE_extendsfdf2 (TARGET_DOUBLE_FLOAT)
#define HAVE_fix_truncsfsi2 (TARGET_HARD_FLOAT)
#define HAVE_fix_truncsfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_HARD_FLOAT)))
#define HAVE_fix_truncdfsi2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_fix_truncdfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)))
#define HAVE_fixuns_truncsfsi2 (TARGET_HARD_FLOAT)
#define HAVE_fixuns_truncsfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_HARD_FLOAT)))
#define HAVE_fixuns_truncdfsi2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_fixuns_truncdfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)))
#define HAVE_floatsisf2 (TARGET_HARD_FLOAT)
#define HAVE_floatdisf2 ((TARGET_HARD_FLOAT) && ((TARGET_HARD_FLOAT) && (TARGET_64BIT)))
#define HAVE_floatsidf2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_floatdidf2 ((TARGET_HARD_FLOAT) && ((TARGET_DOUBLE_FLOAT) && (TARGET_64BIT)))
#define HAVE_floatunssisf2 (TARGET_HARD_FLOAT)
#define HAVE_floatunsdisf2 ((TARGET_HARD_FLOAT) && ((TARGET_HARD_FLOAT) && (TARGET_64BIT)))
#define HAVE_floatunssidf2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_floatunsdidf2 ((TARGET_HARD_FLOAT) && ((TARGET_DOUBLE_FLOAT) && (TARGET_64BIT)))
#define HAVE_lrintsfsi2 (TARGET_HARD_FLOAT)
#define HAVE_lroundsfsi2 (TARGET_HARD_FLOAT)
#define HAVE_lrintsfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_HARD_FLOAT)))
#define HAVE_lroundsfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_HARD_FLOAT)))
#define HAVE_lrintdfsi2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_lrounddfsi2 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_lrintdfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)))
#define HAVE_lrounddfdi2 ((TARGET_HARD_FLOAT) && ((TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)))
#define HAVE_got_loadsi (Pmode == SImode)
#define HAVE_got_loaddi (Pmode == DImode)
#define HAVE_tls_add_tp_lesi (Pmode == SImode)
#define HAVE_tls_add_tp_ledi (Pmode == DImode)
#define HAVE_got_load_tls_gdsi (Pmode == SImode)
#define HAVE_got_load_tls_gddi (Pmode == DImode)
#define HAVE_got_load_tls_iesi (Pmode == SImode)
#define HAVE_got_load_tls_iedi (Pmode == DImode)
#define HAVE_auipcsi (Pmode == SImode)
#define HAVE_auipcdi (Pmode == DImode)
#define HAVE_fence 1
#define HAVE_fence_i (TARGET_ZIFENCEI)
#define HAVE_ashlsi3 1
#define HAVE_ashrsi3 1
#define HAVE_lshrsi3 1
#define HAVE_ashldi3 (TARGET_64BIT)
#define HAVE_ashrdi3 (TARGET_64BIT)
#define HAVE_lshrdi3 (TARGET_64BIT)
#define HAVE_zero_extendsidi2_shifted (TARGET_64BIT && !TARGET_ZBA \
   && ((INTVAL (operands[3]) >> INTVAL (operands[2])) == 0xffffffff))
#define HAVE_jump 1
#define HAVE_indirect_jumpsi (Pmode == SImode)
#define HAVE_indirect_jumpdi (Pmode == DImode)
#define HAVE_tablejumpsi 1
#define HAVE_tablejumpdi (TARGET_64BIT)
#define HAVE_blockage 1
#define HAVE_simple_return 1
#define HAVE_simple_return_internal 1
#define HAVE_eh_set_lr_si (! TARGET_64BIT)
#define HAVE_eh_set_lr_di (TARGET_64BIT)
#define HAVE_eh_return_internal 1
#define HAVE_sibcall_internal (SIBLING_CALL_P (insn))
#define HAVE_sibcall_value_internal (SIBLING_CALL_P (insn))
#define HAVE_call_internal 1
#define HAVE_call_value_internal 1
#define HAVE_nop 1
#define HAVE_trap 1
#define HAVE_gpr_save 1
#define HAVE_gpr_restore 1
#define HAVE_gpr_restore_return 1
#define HAVE_riscv_frflags (TARGET_HARD_FLOAT)
#define HAVE_riscv_fsflags (TARGET_HARD_FLOAT)
#define HAVE_riscv_mret 1
#define HAVE_riscv_sret 1
#define HAVE_riscv_uret 1
#define HAVE_stack_tiesi (!TARGET_64BIT)
#define HAVE_stack_tiedi (TARGET_64BIT)
#define HAVE_stack_protect_set_si 1
#define HAVE_stack_protect_set_di (TARGET_64BIT)
#define HAVE_stack_protect_test_si 1
#define HAVE_stack_protect_test_di (TARGET_64BIT)
#define HAVE_clzsi2 (TARGET_ZBB)
#define HAVE_ctzsi2 (TARGET_ZBB)
#define HAVE_popcountsi2 (TARGET_ZBB)
#define HAVE_clzdi2 (TARGET_64BIT && TARGET_ZBB)
#define HAVE_ctzdi2 (TARGET_64BIT && TARGET_ZBB)
#define HAVE_popcountdi2 (TARGET_64BIT && TARGET_ZBB)
#define HAVE_rotrsi3 (TARGET_ZBB)
#define HAVE_rotrdi3 (TARGET_64BIT && TARGET_ZBB)
#define HAVE_rotrsi3_sext (TARGET_64BIT && TARGET_ZBB)
#define HAVE_rotlsi3 (TARGET_ZBB)
#define HAVE_rotldi3 (TARGET_64BIT && TARGET_ZBB)
#define HAVE_rotlsi3_sext (TARGET_64BIT && TARGET_ZBB)
#define HAVE_bswapsi2 ((TARGET_64BIT && TARGET_ZBB) && (!TARGET_64BIT))
#define HAVE_bswapdi2 ((TARGET_64BIT && TARGET_ZBB) && (TARGET_64BIT))
#define HAVE_sminsi3 ((TARGET_ZBB) && (!TARGET_64BIT))
#define HAVE_uminsi3 ((TARGET_ZBB) && (!TARGET_64BIT))
#define HAVE_smaxsi3 ((TARGET_ZBB) && (!TARGET_64BIT))
#define HAVE_umaxsi3 ((TARGET_ZBB) && (!TARGET_64BIT))
#define HAVE_smindi3 ((TARGET_ZBB) && (TARGET_64BIT))
#define HAVE_umindi3 ((TARGET_ZBB) && (TARGET_64BIT))
#define HAVE_smaxdi3 ((TARGET_ZBB) && (TARGET_64BIT))
#define HAVE_umaxdi3 ((TARGET_ZBB) && (TARGET_64BIT))
#define HAVE_mem_thread_fence_1 1
#define HAVE_atomic_storesi (TARGET_ATOMIC)
#define HAVE_atomic_storedi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_addsi (TARGET_ATOMIC)
#define HAVE_atomic_orsi (TARGET_ATOMIC)
#define HAVE_atomic_xorsi (TARGET_ATOMIC)
#define HAVE_atomic_andsi (TARGET_ATOMIC)
#define HAVE_atomic_adddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_ordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_xordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_anddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_addsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_orsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_xorsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_andsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_adddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_ordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_xordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_anddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_exchangesi (TARGET_ATOMIC)
#define HAVE_atomic_exchangedi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_cas_value_strongsi (TARGET_ATOMIC)
#define HAVE_atomic_cas_value_strongdi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_rvtt_load_immediate (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpassignlreg_int (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpnonimm_dst (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpnonimm_dst_src (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpnonimm_src (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpnonimm_store (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpincrwc (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_gs_l1_load_war (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpgccmov_cc (TARGET_RVTT_GS  && \
   (   register_operand (operands[0], V64SFmode) \
    || reg_or_0_operand (operands[1], V64SFmode)))
#define HAVE_rvtt_gs_sfpassign_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfppreservelreg0_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfppreservelreg1_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfppreservelreg2_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfppreservelreg3_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpload_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfploadi_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpstore_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmuli_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpaddi_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpdivp2_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpexexp_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpexman_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpabs_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmov_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfplz_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmul_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpadd_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpiadd_v_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpiadd_i_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpshft_v (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpshft_i_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpand (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpor (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpnot_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetexp_v (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetman_v (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetsgn_v (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetexp_i_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetman_i_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetsgn_i_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmad_int (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetcc_i (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetcc_v (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpencc (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpcompc (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfppushc (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfppopc (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfplut (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpnop (TARGET_RVTT_GS)
#define HAVE_rvtt_wh_sfpgccmov_cc (TARGET_RVTT_WH  && \
   (   register_operand (operands[0], V64SFmode) \
    || reg_or_0_operand (operands[1], V64SFmode)))
#define HAVE_rvtt_wh_sfpassign_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg0_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg1_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg2_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg3_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg4_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg5_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg6_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppreservelreg7_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpload_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfploadi_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstore_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmuli_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpaddi_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpdivp2_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpexexp_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpexman_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpabs_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmov_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfplz_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmul_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpadd_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpiadd_v_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpiadd_i_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft_i_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpand (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpor (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxor (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpnot_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpcast_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft2_e_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstochrnd_i_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstochrnd_v_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetexp_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetman_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetsgn_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetexp_i_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetsgn_i_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetman_i_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmad_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetcc_i (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetcc_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpencc (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpcompc (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppushc (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfppopc (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfplut (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfplutfp32_3r (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfplutfp32_6r (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpconfig_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpreplay (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpswap_int (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfptransp (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft2_g (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft2_ge (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpnop (TARGET_RVTT_WH)
#define HAVE_rvtt_bh_sfpgccmov_cc (TARGET_RVTT_BH  && \
   (   register_operand (operands[0], V64SFmode) \
    || reg_or_0_operand (operands[1], V64SFmode)))
#define HAVE_rvtt_bh_sfpassign_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg0_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg1_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg2_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg3_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg4_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg5_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg6_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppreservelreg7_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpload_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfploadi_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstore_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmuli_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpaddi_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpdivp2_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpexexp_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpexman_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpabs_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmov_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfplz_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmul_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpadd_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpiadd_v_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpiadd_i_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft_i_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpand (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpor (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxor (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpnot_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpcast_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft2_e_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstochrnd_i_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstochrnd_v_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetexp_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetman_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetsgn_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetexp_i_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetsgn_i_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetman_i_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmad_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetcc_i (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetcc_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpencc (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpcompc (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppushc (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfppopc (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfplut (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfplutfp32_3r (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfplutfp32_6r (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpconfig_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpreplay (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpswap_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfptransp (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft2_g (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft2_ge (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpnop (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmul24_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfparecip_int (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpgt (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfple (TARGET_RVTT_BH)
#define HAVE_addvsi4 1
#define HAVE_addvdi4 (TARGET_64BIT)
#define HAVE_uaddvsi4 1
#define HAVE_uaddvdi4 (TARGET_64BIT)
#define HAVE_subvsi4 1
#define HAVE_subvdi4 (TARGET_64BIT)
#define HAVE_usubvsi4 1
#define HAVE_usubvdi4 (TARGET_64BIT)
#define HAVE_mulvsi4 (TARGET_MUL)
#define HAVE_mulvdi4 ((TARGET_MUL) && (TARGET_64BIT))
#define HAVE_umulvsi4 (TARGET_MUL)
#define HAVE_umulvdi4 ((TARGET_MUL) && (TARGET_64BIT))
#define HAVE_mulditi3 (TARGET_MUL && TARGET_64BIT)
#define HAVE_umulditi3 (TARGET_MUL && TARGET_64BIT)
#define HAVE_usmulditi3 (TARGET_MUL && TARGET_64BIT)
#define HAVE_mulsidi3 (TARGET_MUL && !TARGET_64BIT)
#define HAVE_umulsidi3 (TARGET_MUL && !TARGET_64BIT)
#define HAVE_usmulsidi3 (TARGET_MUL && !TARGET_64BIT)
#define HAVE_zero_extendsidi2 (TARGET_64BIT)
#define HAVE_zero_extendhisi2 1
#define HAVE_zero_extendhidi2 (TARGET_64BIT)
#define HAVE_extendqihi2 1
#define HAVE_extendqisi2 1
#define HAVE_extendqidi2 (TARGET_64BIT)
#define HAVE_extendhihi2 1
#define HAVE_extendhisi2 1
#define HAVE_extendhidi2 (TARGET_64BIT)
#define HAVE_movdi 1
#define HAVE_movsi 1
#define HAVE_movhi 1
#define HAVE_movqi 1
#define HAVE_movsf 1
#define HAVE_movdf 1
#define HAVE_cpymemsi 1
#define HAVE_clear_cache 1
#define HAVE_movsicc (TARGET_SFB_ALU)
#define HAVE_movdicc ((TARGET_SFB_ALU) && (TARGET_64BIT))
#define HAVE_condjump 1
#define HAVE_cbranchqi4 (TARGET_64BIT)
#define HAVE_cbranchsi4 1
#define HAVE_cbranchdi4 (TARGET_64BIT)
#define HAVE_cbranchsf4 (TARGET_HARD_FLOAT)
#define HAVE_cbranchdf4 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_cstoresi4 1
#define HAVE_cstoredi4 (TARGET_64BIT)
#define HAVE_cstoresf4 (TARGET_HARD_FLOAT)
#define HAVE_cstoredf4 ((TARGET_HARD_FLOAT) && (TARGET_DOUBLE_FLOAT))
#define HAVE_flt_quietsfsi4 ((TARGET_HARD_FLOAT) && ((((!TARGET_64BIT) && (TARGET_HARD_FLOAT)) && (TARGET_HARD_FLOAT)) && (!TARGET_64BIT)))
#define HAVE_fle_quietsfsi4 ((TARGET_HARD_FLOAT) && ((((!TARGET_64BIT) && (TARGET_HARD_FLOAT)) && (TARGET_HARD_FLOAT)) && (!TARGET_64BIT)))
#define HAVE_flt_quietsfdi4 ((TARGET_HARD_FLOAT) && ((((TARGET_64BIT) && (TARGET_HARD_FLOAT)) && (TARGET_HARD_FLOAT)) && (TARGET_64BIT)))
#define HAVE_fle_quietsfdi4 ((TARGET_HARD_FLOAT) && ((((TARGET_64BIT) && (TARGET_HARD_FLOAT)) && (TARGET_HARD_FLOAT)) && (TARGET_64BIT)))
#define HAVE_flt_quietdfsi4 ((TARGET_HARD_FLOAT) && ((((!TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)) && (TARGET_DOUBLE_FLOAT)) && (!TARGET_64BIT)))
#define HAVE_fle_quietdfsi4 ((TARGET_HARD_FLOAT) && ((((!TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)) && (TARGET_DOUBLE_FLOAT)) && (!TARGET_64BIT)))
#define HAVE_flt_quietdfdi4 ((TARGET_HARD_FLOAT) && ((((TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)) && (TARGET_DOUBLE_FLOAT)) && (TARGET_64BIT)))
#define HAVE_fle_quietdfdi4 ((TARGET_HARD_FLOAT) && ((((TARGET_64BIT) && (TARGET_DOUBLE_FLOAT)) && (TARGET_DOUBLE_FLOAT)) && (TARGET_64BIT)))
#define HAVE_indirect_jump 1
#define HAVE_tablejump 1
#define HAVE_prologue 1
#define HAVE_epilogue 1
#define HAVE_sibcall_epilogue 1
#define HAVE_return (riscv_can_use_return_insn ())
#define HAVE_eh_return 1
#define HAVE_sibcall 1
#define HAVE_sibcall_value 1
#define HAVE_call 1
#define HAVE_call_value 1
#define HAVE_untyped_call 1
#define HAVE_restore_stack_nonlocal 1
#define HAVE_get_thread_pointersi (Pmode == SImode)
#define HAVE_get_thread_pointerdi (Pmode == DImode)
#define HAVE_stack_protect_set 1
#define HAVE_stack_protect_test 1
#define HAVE_mem_thread_fence 1
#define HAVE_atomic_compare_and_swapsi (TARGET_ATOMIC)
#define HAVE_atomic_compare_and_swapdi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_test_and_set (TARGET_ATOMIC)
#define HAVE_movv64sf 1
#define HAVE_rvtt_sfpassignlreg (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpxicmps (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpxicmpv (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpxvif (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpxbool (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpxcondb (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_sfpxcondi (TARGET_RVTT_GS || TARGET_RVTT_WH || TARGET_RVTT_BH)
#define HAVE_rvtt_gs_sfppreservelreg (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpload (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpload_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpxloadi (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpxloadi_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpstore (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmuli (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpaddi (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpdivp2 (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpdivp2_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpexexp (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpexman (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpabs (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmov (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfplz (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpexexp_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpexman_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpabs_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmov_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfplz_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmul (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpadd (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmul_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpadd_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpxiadd_v (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpxiadd_i (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpxiadd_i_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpshft_i (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpnot (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpnot_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetexp_i (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetman_i (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetsgn_i (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetexp_i_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetman_i_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpsetsgn_i_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmad (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpmad_lv (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpxfcmps (TARGET_RVTT_GS)
#define HAVE_rvtt_gs_sfpxfcmpv (TARGET_RVTT_GS)
#define HAVE_rvtt_wh_sfppreservelreg (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpload (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpload_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxloadi (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxloadi_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstore (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmuli (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpaddi (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpdivp2 (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpdivp2_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpexexp (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpexman (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpabs (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmov (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfplz (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpexexp_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpexman_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpabs_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmov_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfplz_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmul (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpadd (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmul_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpadd_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxiadd_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxiadd_i (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxiadd_i_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft_i (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpnot (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpnot_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpcast (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpcast_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft2_e (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpshft2_e_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstochrnd_i (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstochrnd_i_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstochrnd_v (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpstochrnd_v_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetexp_i (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetsgn_i (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetexp_i_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetsgn_i_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetman_i (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpsetman_i_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmad (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpmad_lv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxfcmps (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpxfcmpv (TARGET_RVTT_WH)
#define HAVE_rvtt_wh_sfpswap (TARGET_RVTT_WH)
#define HAVE_rvtt_bh_sfppreservelreg (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpload (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpload_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxloadi (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxloadi_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstore (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmuli (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpaddi (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpdivp2 (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpdivp2_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpexexp (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpexman (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpabs (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmov (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfplz (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpexexp_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpexman_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpabs_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmov_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfplz_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmul (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpadd (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmul_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpadd_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxiadd_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxiadd_i (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxiadd_i_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft_i (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpnot (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpnot_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpcast (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpcast_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft2_e (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpshft2_e_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstochrnd_i (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstochrnd_i_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstochrnd_v (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpstochrnd_v_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetexp_i (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetsgn_i (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetexp_i_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetsgn_i_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetman_i (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpsetman_i_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmad (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmad_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxfcmps (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpxfcmpv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpswap (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmul24 (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfpmul24_lv (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfparecip (TARGET_RVTT_BH)
#define HAVE_rvtt_bh_sfparecip_lv (TARGET_RVTT_BH)
extern rtx        gen_addsf3                       (rtx, rtx, rtx);
extern rtx        gen_adddf3                       (rtx, rtx, rtx);
extern rtx        gen_addsi3                       (rtx, rtx, rtx);
extern rtx        gen_adddi3                       (rtx, rtx, rtx);
extern rtx        gen_subsf3                       (rtx, rtx, rtx);
extern rtx        gen_subdf3                       (rtx, rtx, rtx);
extern rtx        gen_subdi3                       (rtx, rtx, rtx);
extern rtx        gen_subsi3                       (rtx, rtx, rtx);
extern rtx        gen_negdi2                       (rtx, rtx);
extern rtx        gen_negsi2                       (rtx, rtx);
extern rtx        gen_mulsf3                       (rtx, rtx, rtx);
extern rtx        gen_muldf3                       (rtx, rtx, rtx);
extern rtx        gen_mulsi3                       (rtx, rtx, rtx);
extern rtx        gen_muldi3                       (rtx, rtx, rtx);
extern rtx        gen_smuldi3_highpart             (rtx, rtx, rtx);
extern rtx        gen_umuldi3_highpart             (rtx, rtx, rtx);
extern rtx        gen_usmuldi3_highpart            (rtx, rtx, rtx);
extern rtx        gen_smulsi3_highpart             (rtx, rtx, rtx);
extern rtx        gen_umulsi3_highpart             (rtx, rtx, rtx);
extern rtx        gen_usmulsi3_highpart            (rtx, rtx, rtx);
extern rtx        gen_divsi3                       (rtx, rtx, rtx);
extern rtx        gen_udivsi3                      (rtx, rtx, rtx);
extern rtx        gen_modsi3                       (rtx, rtx, rtx);
extern rtx        gen_umodsi3                      (rtx, rtx, rtx);
extern rtx        gen_divdi3                       (rtx, rtx, rtx);
extern rtx        gen_udivdi3                      (rtx, rtx, rtx);
extern rtx        gen_moddi3                       (rtx, rtx, rtx);
extern rtx        gen_umoddi3                      (rtx, rtx, rtx);
extern rtx        gen_divsf3                       (rtx, rtx, rtx);
extern rtx        gen_divdf3                       (rtx, rtx, rtx);
extern rtx        gen_sqrtsf2                      (rtx, rtx);
extern rtx        gen_sqrtdf2                      (rtx, rtx);
extern rtx        gen_fmasf4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_fmadf4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_fmssf4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_fmsdf4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_fnmssf4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_fnmsdf4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_fnmasf4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_fnmadf4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_abssf2                       (rtx, rtx);
extern rtx        gen_absdf2                       (rtx, rtx);
extern rtx        gen_copysignsf3                  (rtx, rtx, rtx);
extern rtx        gen_copysigndf3                  (rtx, rtx, rtx);
extern rtx        gen_negsf2                       (rtx, rtx);
extern rtx        gen_negdf2                       (rtx, rtx);
extern rtx        gen_sminsf3                      (rtx, rtx, rtx);
extern rtx        gen_smindf3                      (rtx, rtx, rtx);
extern rtx        gen_smaxsf3                      (rtx, rtx, rtx);
extern rtx        gen_smaxdf3                      (rtx, rtx, rtx);
extern rtx        gen_andsi3                       (rtx, rtx, rtx);
extern rtx        gen_iorsi3                       (rtx, rtx, rtx);
extern rtx        gen_xorsi3                       (rtx, rtx, rtx);
extern rtx        gen_anddi3                       (rtx, rtx, rtx);
extern rtx        gen_iordi3                       (rtx, rtx, rtx);
extern rtx        gen_xordi3                       (rtx, rtx, rtx);
extern rtx        gen_one_cmplsi2                  (rtx, rtx);
extern rtx        gen_one_cmpldi2                  (rtx, rtx);
extern rtx        gen_truncdfsf2                   (rtx, rtx);
extern rtx        gen_zero_extendqihi2             (rtx, rtx);
extern rtx        gen_zero_extendqisi2             (rtx, rtx);
extern rtx        gen_zero_extendqidi2             (rtx, rtx);
extern rtx        gen_extendsidi2                  (rtx, rtx);
extern rtx        gen_extendsfdf2                  (rtx, rtx);
extern rtx        gen_fix_truncsfsi2               (rtx, rtx);
extern rtx        gen_fix_truncsfdi2               (rtx, rtx);
extern rtx        gen_fix_truncdfsi2               (rtx, rtx);
extern rtx        gen_fix_truncdfdi2               (rtx, rtx);
extern rtx        gen_fixuns_truncsfsi2            (rtx, rtx);
extern rtx        gen_fixuns_truncsfdi2            (rtx, rtx);
extern rtx        gen_fixuns_truncdfsi2            (rtx, rtx);
extern rtx        gen_fixuns_truncdfdi2            (rtx, rtx);
extern rtx        gen_floatsisf2                   (rtx, rtx);
extern rtx        gen_floatdisf2                   (rtx, rtx);
extern rtx        gen_floatsidf2                   (rtx, rtx);
extern rtx        gen_floatdidf2                   (rtx, rtx);
extern rtx        gen_floatunssisf2                (rtx, rtx);
extern rtx        gen_floatunsdisf2                (rtx, rtx);
extern rtx        gen_floatunssidf2                (rtx, rtx);
extern rtx        gen_floatunsdidf2                (rtx, rtx);
extern rtx        gen_lrintsfsi2                   (rtx, rtx);
extern rtx        gen_lroundsfsi2                  (rtx, rtx);
extern rtx        gen_lrintsfdi2                   (rtx, rtx);
extern rtx        gen_lroundsfdi2                  (rtx, rtx);
extern rtx        gen_lrintdfsi2                   (rtx, rtx);
extern rtx        gen_lrounddfsi2                  (rtx, rtx);
extern rtx        gen_lrintdfdi2                   (rtx, rtx);
extern rtx        gen_lrounddfdi2                  (rtx, rtx);
extern rtx        gen_got_loadsi                   (rtx, rtx);
extern rtx        gen_got_loaddi                   (rtx, rtx);
extern rtx        gen_tls_add_tp_lesi              (rtx, rtx, rtx, rtx);
extern rtx        gen_tls_add_tp_ledi              (rtx, rtx, rtx, rtx);
extern rtx        gen_got_load_tls_gdsi            (rtx, rtx);
extern rtx        gen_got_load_tls_gddi            (rtx, rtx);
extern rtx        gen_got_load_tls_iesi            (rtx, rtx);
extern rtx        gen_got_load_tls_iedi            (rtx, rtx);
extern rtx        gen_auipcsi                      (rtx, rtx, rtx);
extern rtx        gen_auipcdi                      (rtx, rtx, rtx);
extern rtx        gen_fence                        (void);
extern rtx        gen_fence_i                      (void);
extern rtx        gen_ashlsi3                      (rtx, rtx, rtx);
extern rtx        gen_ashrsi3                      (rtx, rtx, rtx);
extern rtx        gen_lshrsi3                      (rtx, rtx, rtx);
extern rtx        gen_ashldi3                      (rtx, rtx, rtx);
extern rtx        gen_ashrdi3                      (rtx, rtx, rtx);
extern rtx        gen_lshrdi3                      (rtx, rtx, rtx);
extern rtx        gen_zero_extendsidi2_shifted     (rtx, rtx, rtx, rtx);
extern rtx        gen_jump                         (rtx);
extern rtx        gen_indirect_jumpsi              (rtx);
extern rtx        gen_indirect_jumpdi              (rtx);
extern rtx        gen_tablejumpsi                  (rtx, rtx, rtx);
extern rtx        gen_tablejumpdi                  (rtx, rtx, rtx);
extern rtx        gen_blockage                     (void);
extern rtx        gen_simple_return                (void);
extern rtx        gen_simple_return_internal       (rtx);
extern rtx        gen_eh_set_lr_si                 (rtx);
extern rtx        gen_eh_set_lr_di                 (rtx);
extern rtx        gen_eh_return_internal           (void);
extern rtx        gen_sibcall_internal             (rtx, rtx);
extern rtx        gen_sibcall_value_internal       (rtx, rtx, rtx);
extern rtx        gen_call_internal                (rtx, rtx);
extern rtx        gen_call_value_internal          (rtx, rtx, rtx);
extern rtx        gen_nop                          (void);
extern rtx        gen_trap                         (void);
extern rtx        gen_gpr_save                     (rtx, rtx);
extern rtx        gen_gpr_restore                  (rtx);
extern rtx        gen_gpr_restore_return           (rtx);
extern rtx        gen_riscv_frflags                (rtx);
extern rtx        gen_riscv_fsflags                (rtx);
extern rtx        gen_riscv_mret                   (void);
extern rtx        gen_riscv_sret                   (void);
extern rtx        gen_riscv_uret                   (void);
extern rtx        gen_stack_tiesi                  (rtx, rtx);
extern rtx        gen_stack_tiedi                  (rtx, rtx);
extern rtx        gen_stack_protect_set_si         (rtx, rtx);
extern rtx        gen_stack_protect_set_di         (rtx, rtx);
extern rtx        gen_stack_protect_test_si        (rtx, rtx, rtx);
extern rtx        gen_stack_protect_test_di        (rtx, rtx, rtx);
extern rtx        gen_clzsi2                       (rtx, rtx);
extern rtx        gen_ctzsi2                       (rtx, rtx);
extern rtx        gen_popcountsi2                  (rtx, rtx);
extern rtx        gen_clzdi2                       (rtx, rtx);
extern rtx        gen_ctzdi2                       (rtx, rtx);
extern rtx        gen_popcountdi2                  (rtx, rtx);
extern rtx        gen_rotrsi3                      (rtx, rtx, rtx);
extern rtx        gen_rotrdi3                      (rtx, rtx, rtx);
extern rtx        gen_rotrsi3_sext                 (rtx, rtx, rtx);
extern rtx        gen_rotlsi3                      (rtx, rtx, rtx);
extern rtx        gen_rotldi3                      (rtx, rtx, rtx);
extern rtx        gen_rotlsi3_sext                 (rtx, rtx, rtx);
extern rtx        gen_bswapsi2                     (rtx, rtx);
extern rtx        gen_bswapdi2                     (rtx, rtx);
extern rtx        gen_sminsi3                      (rtx, rtx, rtx);
extern rtx        gen_uminsi3                      (rtx, rtx, rtx);
extern rtx        gen_smaxsi3                      (rtx, rtx, rtx);
extern rtx        gen_umaxsi3                      (rtx, rtx, rtx);
extern rtx        gen_smindi3                      (rtx, rtx, rtx);
extern rtx        gen_umindi3                      (rtx, rtx, rtx);
extern rtx        gen_smaxdi3                      (rtx, rtx, rtx);
extern rtx        gen_umaxdi3                      (rtx, rtx, rtx);
extern rtx        gen_mem_thread_fence_1           (rtx, rtx);
extern rtx        gen_atomic_storesi               (rtx, rtx, rtx);
extern rtx        gen_atomic_storedi               (rtx, rtx, rtx);
extern rtx        gen_atomic_addsi                 (rtx, rtx, rtx);
extern rtx        gen_atomic_orsi                  (rtx, rtx, rtx);
extern rtx        gen_atomic_xorsi                 (rtx, rtx, rtx);
extern rtx        gen_atomic_andsi                 (rtx, rtx, rtx);
extern rtx        gen_atomic_adddi                 (rtx, rtx, rtx);
extern rtx        gen_atomic_ordi                  (rtx, rtx, rtx);
extern rtx        gen_atomic_xordi                 (rtx, rtx, rtx);
extern rtx        gen_atomic_anddi                 (rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_addsi           (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_orsi            (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_xorsi           (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_andsi           (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_adddi           (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_ordi            (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_xordi           (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_anddi           (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_exchangesi            (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_exchangedi            (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_cas_value_strongsi    (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_cas_value_strongdi    (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_load_immediate          (rtx, rtx);
extern rtx        gen_rvtt_sfpassignlreg_int       (rtx);
extern rtx        gen_rvtt_sfpnonimm_dst           (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_sfpnonimm_dst_src       (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_sfpnonimm_src           (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_sfpnonimm_store         (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_sfpincrwc               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_l1_load_war          (rtx);
extern rtx        gen_rvtt_gs_sfpgccmov_cc         (rtx, rtx);
extern rtx        gen_rvtt_gs_sfpassign_lv         (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfppreservelreg0_int (rtx);
extern rtx        gen_rvtt_gs_sfppreservelreg1_int (rtx);
extern rtx        gen_rvtt_gs_sfppreservelreg2_int (rtx);
extern rtx        gen_rvtt_gs_sfppreservelreg3_int (rtx);
extern rtx        gen_rvtt_gs_sfpload_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfploadi_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpstore_int         (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmuli_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpaddi_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpdivp2_int         (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpexexp_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpexman_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpabs_int           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmov_int           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfplz_int            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmul_int           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpadd_int           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpiadd_v_int        (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpiadd_i_int        (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpshft_v            (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpshft_i_int        (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpand               (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpor                (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpnot_int           (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetexp_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetman_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetsgn_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetexp_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetman_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetsgn_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmad_int           (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetcc_i           (rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetcc_v           (rtx, rtx);
extern rtx        gen_rvtt_gs_sfpencc              (rtx, rtx);
extern rtx        gen_rvtt_gs_sfpcompc             (void);
extern rtx        gen_rvtt_gs_sfppushc             (void);
extern rtx        gen_rvtt_gs_sfppopc              (void);
extern rtx        gen_rvtt_gs_sfplut               (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpnop               (void);
extern rtx        gen_rvtt_wh_sfpgccmov_cc         (rtx, rtx);
extern rtx        gen_rvtt_wh_sfpassign_lv         (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg0_int (rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg1_int (rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg2_int (rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg3_int (rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg4_int (rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg5_int (rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg6_int (rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg7_int (rtx);
extern rtx        gen_rvtt_wh_sfpload_int          (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfploadi_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstore_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmuli_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpaddi_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpdivp2_int         (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpexexp_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpexman_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpabs_int           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmov_int           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfplz_int            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmul_int           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpadd_int           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpiadd_v_int        (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpiadd_i_int        (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft_v            (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft_i_int        (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpand               (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpor                (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxor               (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpnot_int           (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpcast_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft2_e_int       (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstochrnd_i_int    (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstochrnd_v_int    (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetexp_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetman_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetsgn_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetexp_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetsgn_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetman_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmad_int           (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetcc_i           (rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetcc_v           (rtx, rtx);
extern rtx        gen_rvtt_wh_sfpencc              (rtx, rtx);
extern rtx        gen_rvtt_wh_sfpcompc             (void);
extern rtx        gen_rvtt_wh_sfppushc             (rtx);
extern rtx        gen_rvtt_wh_sfppopc              (rtx);
extern rtx        gen_rvtt_wh_sfplut               (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfplutfp32_3r        (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfplutfp32_6r        (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpconfig_v          (rtx, rtx);
extern rtx        gen_rvtt_wh_sfpreplay            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpswap_int          (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfptransp            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft2_g           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft2_ge          (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpnop               (void);
extern rtx        gen_rvtt_bh_sfpgccmov_cc         (rtx, rtx);
extern rtx        gen_rvtt_bh_sfpassign_lv         (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg0_int (rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg1_int (rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg2_int (rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg3_int (rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg4_int (rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg5_int (rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg6_int (rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg7_int (rtx);
extern rtx        gen_rvtt_bh_sfpload_int          (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfploadi_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstore_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmuli_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpaddi_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpdivp2_int         (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpexexp_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpexman_int         (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpabs_int           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmov_int           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfplz_int            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmul_int           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpadd_int           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpiadd_v_int        (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpiadd_i_int        (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft_v            (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft_i_int        (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpand               (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpor                (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxor               (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpnot_int           (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpcast_int          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft2_e_int       (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstochrnd_i_int    (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstochrnd_v_int    (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetexp_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetman_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetsgn_v          (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetexp_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetsgn_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetman_i_int      (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmad_int           (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetcc_i           (rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetcc_v           (rtx, rtx);
extern rtx        gen_rvtt_bh_sfpencc              (rtx, rtx);
extern rtx        gen_rvtt_bh_sfpcompc             (void);
extern rtx        gen_rvtt_bh_sfppushc             (rtx);
extern rtx        gen_rvtt_bh_sfppopc              (rtx);
extern rtx        gen_rvtt_bh_sfplut               (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfplutfp32_3r        (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfplutfp32_6r        (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpconfig_v          (rtx, rtx);
extern rtx        gen_rvtt_bh_sfpreplay            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpswap_int          (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfptransp            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft2_g           (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft2_ge          (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpnop               (void);
extern rtx        gen_rvtt_bh_sfpmul24_int         (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfparecip_int        (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpgt                (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfple                (rtx, rtx, rtx);
extern rtx        gen_addvsi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_addvdi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_uaddvsi4                     (rtx, rtx, rtx, rtx);
extern rtx        gen_uaddvdi4                     (rtx, rtx, rtx, rtx);
extern rtx        gen_subvsi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_subvdi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_usubvsi4                     (rtx, rtx, rtx, rtx);
extern rtx        gen_usubvdi4                     (rtx, rtx, rtx, rtx);
extern rtx        gen_mulvsi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_mulvdi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_umulvsi4                     (rtx, rtx, rtx, rtx);
extern rtx        gen_umulvdi4                     (rtx, rtx, rtx, rtx);
extern rtx        gen_mulditi3                     (rtx, rtx, rtx);
extern rtx        gen_umulditi3                    (rtx, rtx, rtx);
extern rtx        gen_usmulditi3                   (rtx, rtx, rtx);
extern rtx        gen_mulsidi3                     (rtx, rtx, rtx);
extern rtx        gen_umulsidi3                    (rtx, rtx, rtx);
extern rtx        gen_usmulsidi3                   (rtx, rtx, rtx);
extern rtx        gen_zero_extendsidi2             (rtx, rtx);
extern rtx        gen_zero_extendhisi2             (rtx, rtx);
extern rtx        gen_zero_extendhidi2             (rtx, rtx);
extern rtx        gen_extendqihi2                  (rtx, rtx);
extern rtx        gen_extendqisi2                  (rtx, rtx);
extern rtx        gen_extendqidi2                  (rtx, rtx);
extern rtx        gen_extendhihi2                  (rtx, rtx);
extern rtx        gen_extendhisi2                  (rtx, rtx);
extern rtx        gen_extendhidi2                  (rtx, rtx);
extern rtx        gen_movdi                        (rtx, rtx);
extern rtx        gen_movsi                        (rtx, rtx);
extern rtx        gen_movhi                        (rtx, rtx);
extern rtx        gen_movqi                        (rtx, rtx);
extern rtx        gen_movsf                        (rtx, rtx);
extern rtx        gen_movdf                        (rtx, rtx);
extern rtx        gen_cpymemsi                     (rtx, rtx, rtx, rtx);
extern rtx        gen_clear_cache                  (rtx, rtx);
extern rtx        gen_movsicc                      (rtx, rtx, rtx, rtx);
extern rtx        gen_movdicc                      (rtx, rtx, rtx, rtx);
extern rtx        gen_condjump                     (rtx, rtx);
extern rtx        gen_cbranchqi4                   (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchsi4                   (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchdi4                   (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchsf4                   (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchdf4                   (rtx, rtx, rtx, rtx);
extern rtx        gen_cstoresi4                    (rtx, rtx, rtx, rtx);
extern rtx        gen_cstoredi4                    (rtx, rtx, rtx, rtx);
extern rtx        gen_cstoresf4                    (rtx, rtx, rtx, rtx);
extern rtx        gen_cstoredf4                    (rtx, rtx, rtx, rtx);
extern rtx        gen_flt_quietsfsi4               (rtx, rtx, rtx);
extern rtx        gen_fle_quietsfsi4               (rtx, rtx, rtx);
extern rtx        gen_flt_quietsfdi4               (rtx, rtx, rtx);
extern rtx        gen_fle_quietsfdi4               (rtx, rtx, rtx);
extern rtx        gen_flt_quietdfsi4               (rtx, rtx, rtx);
extern rtx        gen_fle_quietdfsi4               (rtx, rtx, rtx);
extern rtx        gen_flt_quietdfdi4               (rtx, rtx, rtx);
extern rtx        gen_fle_quietdfdi4               (rtx, rtx, rtx);
extern rtx        gen_indirect_jump                (rtx);
extern rtx        gen_tablejump                    (rtx, rtx, rtx);
extern rtx        gen_prologue                     (void);
extern rtx        gen_epilogue                     (void);
extern rtx        gen_sibcall_epilogue             (void);
extern rtx        gen_return                       (void);
extern rtx        gen_eh_return                    (rtx);
extern rtx        gen_sibcall                      (rtx, rtx, rtx, rtx);
extern rtx        gen_sibcall_value                (rtx, rtx, rtx, rtx);
extern rtx        gen_call                         (rtx, rtx, rtx, rtx);
extern rtx        gen_call_value                   (rtx, rtx, rtx, rtx);
extern rtx        gen_untyped_call                 (rtx, rtx, rtx);
extern rtx        gen_restore_stack_nonlocal       (rtx, rtx);
extern rtx        gen_get_thread_pointersi         (rtx);
extern rtx        gen_get_thread_pointerdi         (rtx);
extern rtx        gen_stack_protect_set            (rtx, rtx);
extern rtx        gen_stack_protect_test           (rtx, rtx, rtx);
extern rtx        gen_mem_thread_fence             (rtx);
extern rtx        gen_atomic_compare_and_swapsi    (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_compare_and_swapdi    (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_test_and_set          (rtx, rtx, rtx);
extern rtx        gen_movv64sf                     (rtx, rtx);
extern rtx        gen_rvtt_sfpassignlreg           (rtx, rtx);
extern rtx        gen_rvtt_sfpxicmps               (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_sfpxicmpv               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_sfpxvif                 (rtx);
extern rtx        gen_rvtt_sfpxbool                (rtx, rtx);
extern rtx        gen_rvtt_sfpxcondb               (rtx, rtx);
extern rtx        gen_rvtt_sfpxcondi               (rtx, rtx);
extern rtx        gen_rvtt_gs_sfppreservelreg      (rtx, rtx);
extern rtx        gen_rvtt_gs_sfpload              (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpload_lv           (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpxloadi            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpxloadi_lv         (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpstore             (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmuli              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpaddi              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpdivp2             (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpdivp2_lv          (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpexexp             (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpexman             (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpabs               (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmov               (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfplz                (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpexexp_lv          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpexman_lv          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpabs_lv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmov_lv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfplz_lv             (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmul               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpadd               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmul_lv            (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpadd_lv            (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpxiadd_v           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpxiadd_i           (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpxiadd_i_lv        (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpshft_i            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpnot               (rtx, rtx);
extern rtx        gen_rvtt_gs_sfpnot_lv            (rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetexp_i          (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetman_i          (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetsgn_i          (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetexp_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetman_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpsetsgn_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmad               (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpmad_lv            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpxfcmps            (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_gs_sfpxfcmpv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfppreservelreg      (rtx, rtx);
extern rtx        gen_rvtt_wh_sfpload              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpload_lv           (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxloadi            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxloadi_lv         (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstore             (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmuli              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpaddi              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpdivp2             (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpdivp2_lv          (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpexexp             (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpexman             (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpabs               (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmov               (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfplz                (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpexexp_lv          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpexman_lv          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpabs_lv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmov_lv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfplz_lv             (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmul               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpadd               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmul_lv            (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpadd_lv            (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxiadd_v           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxiadd_i           (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxiadd_i_lv        (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft_i            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpnot               (rtx, rtx);
extern rtx        gen_rvtt_wh_sfpnot_lv            (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpcast              (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpcast_lv           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft2_e           (rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpshft2_e_lv        (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstochrnd_i        (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstochrnd_i_lv     (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstochrnd_v        (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpstochrnd_v_lv     (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetexp_i          (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetsgn_i          (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetexp_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetsgn_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetman_i          (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpsetman_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmad               (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpmad_lv            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxfcmps            (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpxfcmpv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_wh_sfpswap              (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfppreservelreg      (rtx, rtx);
extern rtx        gen_rvtt_bh_sfpload              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpload_lv           (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxloadi            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxloadi_lv         (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstore             (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmuli              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpaddi              (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpdivp2             (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpdivp2_lv          (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpexexp             (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpexman             (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpabs               (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmov               (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfplz                (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpexexp_lv          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpexman_lv          (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpabs_lv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmov_lv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfplz_lv             (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmul               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpadd               (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmul_lv            (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpadd_lv            (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxiadd_v           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxiadd_i           (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxiadd_i_lv        (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft_i            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpnot               (rtx, rtx);
extern rtx        gen_rvtt_bh_sfpnot_lv            (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpcast              (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpcast_lv           (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft2_e           (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpshft2_e_lv        (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstochrnd_i        (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstochrnd_i_lv     (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstochrnd_v        (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpstochrnd_v_lv     (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetexp_i          (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetsgn_i          (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetexp_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetsgn_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetman_i          (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpsetman_i_lv       (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmad               (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmad_lv            (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxfcmps            (rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpxfcmpv            (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpswap              (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmul24             (rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfpmul24_lv          (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfparecip            (rtx, rtx, rtx);
extern rtx        gen_rvtt_bh_sfparecip_lv         (rtx, rtx, rtx, rtx);

#endif /* GCC_INSN_FLAGS_H */
