/**
 * Copyright (c) 2024 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _HEAVY_CONTEXT_PROG_HPP_
#define _HEAVY_CONTEXT_PROG_HPP_

// object includes
#include "HeavyContext.hpp"
#include "HvControlDelay.h"
#include "HvControlSystem.h"
#include "HvControlUnop.h"
#include "HvControlBinop.h"
#include "HvSignalPhasor.h"
#include "HvControlVar.h"
#include "HvSignalVar.h"
#include "HvControlIf.h"
#include "HvSignalBiquad.h"
#include "HvControlPrint.h"
#include "HvMath.h"
#include "HvSignalLine.h"
#include "HvControlCast.h"
#include "HvControlSlice.h"

class Heavy_prog : public HeavyContext {

 public:
  Heavy_prog(double sampleRate, int poolKb=10, int inQueueKb=2, int outQueueKb=0);
  ~Heavy_prog();

  const char *getName() override { return "prog"; }
  int getNumInputChannels() override { return 0; }
  int getNumOutputChannels() override { return 2; }

  int process(float **inputBuffers, float **outputBuffer, int n) override;
  int processInline(float *inputBuffers, float *outputBuffer, int n) override;
  int processInlineInterleaved(float *inputBuffers, float *outputBuffer, int n) override;

  int getParameterInfo(int index, HvParameterInfo *info) override;
  struct Parameter {
    struct In {
      enum ParameterIn : hv_uint32_t {
        DETUNE = 0x3861AA6D, // detune
        FREQ = 0x345FC008, // freq
        OSCFREQ = 0x204E6574, // oscFreq
      };
    };
  };

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void cVar_ryTE9SQW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_qfIIgGgn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_HIB95DBA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_KAM2X3oZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_yBpdpZiA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rhrgu9YQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_B8vjFc42_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_UDmcWmgT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_WgpeDiSC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_LpThHcQx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_tTClnEgh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_hhRSrzIT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_yJ6KhCHe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_ZIKUaoSO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_iT04GsnA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_uIzKY47O_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_sFpHsvLl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_PJJ1322p_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_7Dc6OVpa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_7nM3lzsg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_pQOXrbps_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_DSCngXZm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_YuLDy9oA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_duOsEG5f_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_zmR01dtl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_rMnCtVzO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_CVHcLZTF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_TuG4gRU1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rnLMx4zR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_QahAJOgy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rCkjDrfy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_NNwHBA2U_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_4kWurzSp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_MeEXjTsp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_LrArsgSJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_EBb62hvA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_w7qCgcuU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_yVce42d4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_alX5tRi0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_dKEfjQWo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_G9icV9sA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_7Oe4Vvc2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_zZkDYSPA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_9iJoKoDN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_nE6V0vNq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_k5aEA6lZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_5ZTMGuIj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_9zuXc68R_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_0UJvkAx7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_6bti4t3Z_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_FRVZDb6Y_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_mpg73U6z_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_d1PMnZA2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_csEYG6Y8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_fDuNFjQQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_1QisVuQV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_9joQmyyG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ZDcQmKM0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_bUcqh7kg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_FsL6l3u7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_kaCItd8m_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_KB2s82jw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ERYAHtWJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ThIWv3qH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_zc4PreZn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_tppkClog_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_PitweSu2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_4LDSIcLi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_cx2WiLJT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_8dvCXpP3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_UblPeu42_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_58a1BxBS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_cGoCU7Fd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_iU8AnlhX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_9UVgbG1b_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_b2GKfMJh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_CqL4H3Qi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_FPWg0ceu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_eDQSu8QI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_tOl4YrjU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_ZMRaMbPY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ZjKRWXkJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_oBWMPC86_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_23baWcpf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_LQ4JEtpM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_tj1nRzDO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_ghNkILuc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_O1eMbIfo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_MXvyqfoc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Urc6bZRX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JwA17CrT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_XaFhYLQm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_vkDwq3MS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Jqzm1Gw4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_POgAUptY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_gWr9DBNH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_T6wQMUqO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_hE3Lv4Zj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_7mqAJ60S_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_QUtbNYXe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_SFfuAuyv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_zVy88w3J_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_DJwHmsqG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_g1FMb36X_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_S1LcWebf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_Z8cQSFxp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_V23fTGkK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_JutUWKty_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_QiIUuGAP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_9WsIDb92_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_I9vvSQra_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_MQauzLH7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rusYI2yi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_6l6tsWEy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_9hWPc9hp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_gw40at0h_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_J83bKGeG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_HGHfh6o0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_5Y8uDTWI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rWj51Mfu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_oy0yOcNA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_PBmUccrf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ZiB32ign_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_vM7AdSEP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_cd6AmpqH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_I1bPZqDS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_bDlvEkdb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Aju1IYr1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Ih5Li0yO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_LzJgXoGi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_xUMkbUbd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_mb6MWesV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_KZ49bWkG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_56I05L6n_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_fJKLxr87_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_UcBtKPmQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_L2Z4fpc9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_lZRC0tUI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_ZSTPQpg8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_9Swrtcgp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_DE6Jz1C6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_q4dgnYqS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_kYKnEKod_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_vFLozoti_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_OTpmX9CC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_mYhSWwzz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_P3RFwxNW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_iafQ0U8p_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_db9SGKpN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_dVwj9ANY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_T7zBx4Nu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_VvJdJZzZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_IT0by13j_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_RqFUKNBz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_uswKoylP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_zHChEKhi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_h6b2fJOx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_bT5wdYPj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_kBYtz8Om_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_U0tziIMb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_OTOwgdma_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_dKTiA6jo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_2r1PUB0J_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_PdHssghP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_TkMmK1LJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_otTdWA7z_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_YHonjQ47_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_CywOFLUv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_ML9hroip_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_ToVtZBP2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_uDrgw2R4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_usePjgmC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_fJfAzJ58_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_HBWAAwww_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_m01msqUy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_7Giwiys2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_uBKbDpL5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_FwmYYSFq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_J7MitxgM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_D53w4PBO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_y42Fxe5u_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_3NoGDk1l_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JKsLYhiv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_CToj5eET_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_bJnJhvt0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_k4HVKYx6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rqiNhOIc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_J1sQKDP8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_hnfYGk1e_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_bfMzhA1V_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ecs7iZCZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_5Y9i65KB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Hk6dcXL4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_YpK0ifep_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_0pqCIpLd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_GxdjkDXR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_9oHijONQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_PsDu1dS1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_CuT3M1Wr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_pBSMvPsu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_xE0v3whT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_t2jJhxCn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_8HcwxaMl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_dQ00dvVK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_UN9BtfWM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_wrfC2wDy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_QpN30Liv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_ij1jLCVz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_aWlG51OD_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_YmwxP2GW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_bmKfE5Gu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_KdtjB2Wf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_PdyPsZZA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_LNusu3Xy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_MQagWwIB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_MEyI9vgm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_Mz9IADzw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_cjosllpx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_BbBN3Xdb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_2rGQlO9G_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_tBmOUCOz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_XVcXxDhN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_bI2oXjHf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_jSsNankV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_sX4TYpdA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_oS5japQ2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_mT7drNaP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_cv1p0zSe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_aRVWUveh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_cdHhTG3r_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_mzCTo5Sa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_S7M0L0Q2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_p8yAdM6C_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_UyqcH1Pw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_nqnscXy9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_5Th7olNo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_fK26C1s0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_n8PUkkoq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_RIlDIKYU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_AI77PzwL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_QkuGfSQM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_d1Dhyt8t_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Z9knXbGR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_96PuBxqC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_K541dnUz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_xAsMO8s9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_JciWCE8H_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_C6iiMwgg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_xt6j2vZp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_yDpG9Y1g_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_CU3FTxVb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_Co4pcFBW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_igM68kTg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_TixPN7T5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_D7wwLQzh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_pqKqEp8f_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_i3b0lqJv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_I9dVUKPQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_p2qq9c04_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_4dWAoRxo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_Rw8chYSl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_rRoKs52Z_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_eozYHLox_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_G2HS1hDt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_AP4W2tGn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_SLkBtg8M_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_c7dtniuP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_FWOe99S0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_ONQDH4pC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_mbt0fPu0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_mvSxVdKn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_w3QcwT7n_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_2uiQf6mR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_kobBzGh5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_pxqcUNsM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_X9r4Ovjp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_PGb3Xwvx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_XLD3lN6y_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_CnSwQOiw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rVkhX7H5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_zfG6TXW9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ZkHh8mCU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ha5WvWzv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_jCYRWMS0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_6h171LmO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_d6bbRP6c_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_VdsbSiTw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_mSZ0HZLq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_CjYpY9Vp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_QXV3JfBC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_xQPNwJ5v_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_mjiNr8lY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_bX8KzyIN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_8E6sKb0L_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_X84aeyAR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_fVDFRWXb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_5qn8a93m_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_5y4CQ1SG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_DrrQRHbZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_uB9y5VVc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_NE9KtYMs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_h346h3ME_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_JPFfiBNs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_Cd2D1eL1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_WgEQlcV8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_K93MUqZA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_3VS3nq1G_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_8ROvlF9E_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_cvD4pqEN_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_Hqs3y7Tj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_hemXwpKW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_FmsqelSG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_HmcZEssK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_nUtyzNav_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_sFnTcmZl_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_KZdiiaBs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_7nBh8vy8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_ducJXvB8_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_WFAClOKM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_Qewso1G8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_orULR668_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_Ud8Q5CmV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_niX2xuUS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_1Oa3asUj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_i14GQwuQ_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_bLAZxrNG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_wKBe7Ovk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_4sL4NBXT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_2uiZ6JHc_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_kqtLrc4a_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_sQyU6sXZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_1Cq9TXnt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_XqpByOrX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_aYWDHUoz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_QTYAzM7D_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_XMlKgdD8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_tlWDCSL5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_5fVQ4AMs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_KH4fAj73_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_MJgn6QqM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_bCSQTkBs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_TFGGs90p_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_0vS00PCf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_v3o1ZYrG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_KpeCIZTC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_unJU57QK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_awzPqM2i_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_DN0z2suc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_XIQvXAHp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_RP75dAoa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_GuCzNiGX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_w43sFxf7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_V4bTJaJr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_ZOQkEYNH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_q4g1KXl7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_5xXU3ejn_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_VX2x1n2f_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_AoXNzBls_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_Bf7Nysqp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_AK3tjBEF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_sCwPzZi3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_QpKUZPis_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_M0zOkD20_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_7DhszMrJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_vIkdJ3MH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_PAEnssjw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_Ee8ssv5I_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_T4NvQIrk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Ex6u9DpX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_BNN31wF3_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cSlice_sIFKTEjy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_WVO8No4n_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_5HnEabZn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_E2pYkKTr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_Xnr7yvbR_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_HjNmV8Ly_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_0JxmXT6k_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_RgJv1kIW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_bgSaBrGb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_pPsGdLJ9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_4L6qFKQr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_LuFBdPUg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_PdZuTyNT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_RRRzi83x_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_mzGeDqRn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JmRjXbmc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_ws7lyiDP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_fGlue7LF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_tkMCzh4q_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_MgNdjdBF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_a00iGGhj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_QDZYi7ql_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_HP7TgnGm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_I3vJ9WKk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_6ayzkQs0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_zwAaPBX7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_lbG5EA58_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_EI1xiieI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_arv6taHI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_htxliDdx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_xDm6XZwN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_LmI0DSyC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ziIzkQhH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_QGCrFTjz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_uNP7rWZx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_001fHcPc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_0fWSOvqi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_qNbqNt3Q_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_6GtRuvfB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_W2Vrvu2w_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cSlice_DfcbhiHe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_53kk02Ay_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_sp3QIAbc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ZdVr1TNR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_QsLnZIRp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_R7dpVxsU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_4N6SktEw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Bz7qHPTi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_HjgLSHyf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_MYEPFcRK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_R3zudfjE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_0W6AYdU0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_LnHfxKLW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_PARtkqGW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_3uCiN09j_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_Qztg4E12_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_sZeJnywv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_96myYRUQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_2xUMxCfG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_9Wj8ua1u_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_VH4sTuxJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_Z5DvwMdG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_TW0sjGKd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_pdHj3YN1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_BbyYEiN5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_76ajcvoG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_nIi8cd76_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalPhasor sPhasor_yQsb2nIb;
  SignalLine sLine_wO3u3tui;
  SignalLine sLine_Jr4j5thQ;
  SignalLine sLine_KxpjAxuF;
  SignalLine sLine_dR45bG2s;
  SignalLine sLine_YsEKG2MS;
  SignalBiquad sBiquad_s_egl1Ng4C;
  SignalPhasor sPhasor_m2VEJZnO;
  SignalLine sLine_h6bPVxoO;
  SignalLine sLine_2kOE3uyS;
  SignalLine sLine_NdyfbPvE;
  SignalLine sLine_mkvCZZvv;
  SignalLine sLine_26t4Armx;
  SignalBiquad sBiquad_s_uPnadhNU;
  ControlVar cVar_ryTE9SQW;
  SignalVarf sVarf_u3GW6FJi;
  ControlVar cVar_qfIIgGgn;
  SignalVarf sVarf_e0srBG24;
  ControlBinop cBinop_rhrgu9YQ;
  SignalVarf sVarf_r4MmzX2O;
  ControlVar cVar_B8vjFc42;
  SignalVarf sVarf_aJl09ddZ;
  ControlBinop cBinop_tTClnEgh;
  ControlVar cVar_hhRSrzIT;
  ControlBinop cBinop_DSCngXZm;
  ControlBinop cBinop_duOsEG5f;
  ControlBinop cBinop_zmR01dtl;
  ControlBinop cBinop_TuG4gRU1;
  ControlBinop cBinop_rnLMx4zR;
  ControlBinop cBinop_QahAJOgy;
  ControlBinop cBinop_rCkjDrfy;
  ControlBinop cBinop_NNwHBA2U;
  ControlBinop cBinop_MeEXjTsp;
  ControlBinop cBinop_EBb62hvA;
  ControlBinop cBinop_w7qCgcuU;
  ControlBinop cBinop_alX5tRi0;
  ControlBinop cBinop_dKEfjQWo;
  ControlBinop cBinop_G9icV9sA;
  ControlBinop cBinop_zZkDYSPA;
  ControlVar cVar_9iJoKoDN;
  ControlVar cVar_nE6V0vNq;
  ControlVar cVar_k5aEA6lZ;
  ControlBinop cBinop_d1PMnZA2;
  ControlBinop cBinop_csEYG6Y8;
  ControlBinop cBinop_fDuNFjQQ;
  ControlBinop cBinop_9joQmyyG;
  ControlBinop cBinop_ZDcQmKM0;
  ControlBinop cBinop_bUcqh7kg;
  ControlBinop cBinop_FsL6l3u7;
  ControlBinop cBinop_kaCItd8m;
  ControlBinop cBinop_KB2s82jw;
  ControlBinop cBinop_ZjKRWXkJ;
  ControlBinop cBinop_23baWcpf;
  ControlBinop cBinop_LQ4JEtpM;
  ControlBinop cBinop_O1eMbIfo;
  ControlBinop cBinop_MXvyqfoc;
  ControlBinop cBinop_Urc6bZRX;
  ControlBinop cBinop_JwA17CrT;
  ControlBinop cBinop_XaFhYLQm;
  ControlBinop cBinop_Jqzm1Gw4;
  ControlBinop cBinop_gWr9DBNH;
  ControlBinop cBinop_T6wQMUqO;
  ControlBinop cBinop_7mqAJ60S;
  ControlBinop cBinop_QUtbNYXe;
  ControlBinop cBinop_SFfuAuyv;
  ControlBinop cBinop_DJwHmsqG;
  ControlVar cVar_g1FMb36X;
  ControlVar cVar_S1LcWebf;
  ControlVar cVar_Z8cQSFxp;
  ControlBinop cBinop_rusYI2yi;
  ControlBinop cBinop_6l6tsWEy;
  ControlBinop cBinop_9hWPc9hp;
  ControlBinop cBinop_J83bKGeG;
  ControlBinop cBinop_HGHfh6o0;
  ControlBinop cBinop_5Y8uDTWI;
  ControlBinop cBinop_rWj51Mfu;
  ControlBinop cBinop_oy0yOcNA;
  ControlBinop cBinop_PBmUccrf;
  ControlVar cVar_KZ49bWkG;
  ControlSlice cSlice_56I05L6n;
  ControlSlice cSlice_fJKLxr87;
  ControlSlice cSlice_UcBtKPmQ;
  ControlVar cVar_L2Z4fpc9;
  ControlIf cIf_9Swrtcgp;
  ControlBinop cBinop_DE6Jz1C6;
  ControlIf cIf_q4dgnYqS;
  ControlIf cIf_kYKnEKod;
  ControlIf cIf_vFLozoti;
  ControlIf cIf_OTpmX9CC;
  ControlBinop cBinop_mYhSWwzz;
  ControlBinop cBinop_P3RFwxNW;
  ControlBinop cBinop_iafQ0U8p;
  ControlBinop cBinop_IT0by13j;
  ControlBinop cBinop_RqFUKNBz;
  ControlBinop cBinop_uswKoylP;
  ControlBinop cBinop_zHChEKhi;
  ControlBinop cBinop_h6b2fJOx;
  ControlVar cVar_kBYtz8Om;
  ControlIf cIf_dKTiA6jo;
  ControlBinop cBinop_2r1PUB0J;
  ControlVar cVar_PdHssghP;
  ControlIf cIf_YHonjQ47;
  ControlBinop cBinop_CywOFLUv;
  ControlSlice cSlice_ML9hroip;
  ControlSlice cSlice_ToVtZBP2;
  ControlSlice cSlice_uDrgw2R4;
  ControlIf cIf_usePjgmC;
  ControlIf cIf_fJfAzJ58;
  ControlIf cIf_HBWAAwww;
  ControlIf cIf_m01msqUy;
  ControlIf cIf_7Giwiys2;
  ControlIf cIf_uBKbDpL5;
  ControlIf cIf_FwmYYSFq;
  ControlIf cIf_J7MitxgM;
  ControlIf cIf_D53w4PBO;
  ControlVar cVar_y42Fxe5u;
  ControlBinop cBinop_3NoGDk1l;
  ControlBinop cBinop_JKsLYhiv;
  ControlBinop cBinop_CToj5eET;
  ControlBinop cBinop_rqiNhOIc;
  ControlBinop cBinop_J1sQKDP8;
  ControlBinop cBinop_YpK0ifep;
  ControlBinop cBinop_0pqCIpLd;
  ControlVar cVar_UN9BtfWM;
  ControlIf cIf_ij1jLCVz;
  ControlBinop cBinop_aWlG51OD;
  ControlVar cVar_YmwxP2GW;
  ControlIf cIf_PdyPsZZA;
  ControlBinop cBinop_LNusu3Xy;
  ControlSlice cSlice_MQagWwIB;
  ControlSlice cSlice_MEyI9vgm;
  ControlSlice cSlice_Mz9IADzw;
  ControlIf cIf_cjosllpx;
  ControlIf cIf_BbBN3Xdb;
  ControlIf cIf_2rGQlO9G;
  ControlIf cIf_tBmOUCOz;
  ControlIf cIf_XVcXxDhN;
  ControlIf cIf_bI2oXjHf;
  ControlIf cIf_jSsNankV;
  ControlIf cIf_sX4TYpdA;
  ControlIf cIf_oS5japQ2;
  ControlVar cVar_mT7drNaP;
  ControlBinop cBinop_cv1p0zSe;
  ControlBinop cBinop_aRVWUveh;
  ControlBinop cBinop_cdHhTG3r;
  ControlBinop cBinop_p8yAdM6C;
  ControlBinop cBinop_UyqcH1Pw;
  ControlBinop cBinop_AI77PzwL;
  ControlBinop cBinop_QkuGfSQM;
  ControlVar cVar_CU3FTxVb;
  ControlIf cIf_TixPN7T5;
  ControlBinop cBinop_D7wwLQzh;
  ControlVar cVar_pqKqEp8f;
  ControlIf cIf_p2qq9c04;
  ControlBinop cBinop_4dWAoRxo;
  ControlSlice cSlice_Rw8chYSl;
  ControlSlice cSlice_rRoKs52Z;
  ControlSlice cSlice_eozYHLox;
  ControlIf cIf_G2HS1hDt;
  ControlIf cIf_AP4W2tGn;
  ControlIf cIf_SLkBtg8M;
  ControlIf cIf_c7dtniuP;
  ControlIf cIf_FWOe99S0;
  ControlIf cIf_ONQDH4pC;
  ControlIf cIf_mbt0fPu0;
  ControlIf cIf_mvSxVdKn;
  ControlIf cIf_w3QcwT7n;
  ControlVar cVar_2uiQf6mR;
  ControlBinop cBinop_kobBzGh5;
  ControlBinop cBinop_pxqcUNsM;
  ControlBinop cBinop_X9r4Ovjp;
  ControlBinop cBinop_CnSwQOiw;
  ControlBinop cBinop_rVkhX7H5;
  ControlBinop cBinop_d6bbRP6c;
  ControlBinop cBinop_VdsbSiTw;
  ControlSlice cSlice_5qn8a93m;
  ControlSlice cSlice_5y4CQ1SG;
  ControlBinop cBinop_DrrQRHbZ;
  ControlBinop cBinop_h346h3ME;
  ControlBinop cBinop_WgEQlcV8;
  ControlBinop cBinop_8ROvlF9E;
  ControlIf cIf_hemXwpKW;
  ControlVar cVar_FmsqelSG;
  ControlVar cVar_HmcZEssK;
  ControlVar cVar_nUtyzNav;
  ControlDelay cDelay_WFAClOKM;
  ControlIf cIf_Ud8Q5CmV;
  ControlBinop cBinop_niX2xuUS;
  ControlVar cVar_1Oa3asUj;
  ControlVar cVar_wKBe7Ovk;
  ControlVar cVar_4sL4NBXT;
  ControlDelay cDelay_kqtLrc4a;
  ControlBinop cBinop_XMlKgdD8;
  ControlBinop cBinop_TFGGs90p;
  ControlBinop cBinop_awzPqM2i;
  ControlBinop cBinop_DN0z2suc;
  ControlVar cVar_RP75dAoa;
  ControlVar cVar_V4bTJaJr;
  ControlDelay cDelay_VX2x1n2f;
  ControlVar cVar_sCwPzZi3;
  ControlBinop cBinop_M0zOkD20;
  ControlBinop cBinop_7DhszMrJ;
  ControlBinop cBinop_vIkdJ3MH;
  ControlVar cVar_PAEnssjw;
  ControlSlice cSlice_sIFKTEjy;
  ControlSlice cSlice_WVO8No4n;
  ControlVar cVar_5HnEabZn;
  ControlVar cVar_E2pYkKTr;
  ControlVar cVar_0JxmXT6k;
  ControlVar cVar_RgJv1kIW;
  ControlVar cVar_bgSaBrGb;
  ControlSlice cSlice_pPsGdLJ9;
  ControlSlice cSlice_4L6qFKQr;
  ControlBinop cBinop_LuFBdPUg;
  ControlBinop cBinop_PdZuTyNT;
  ControlBinop cBinop_RRRzi83x;
  ControlBinop cBinop_mzGeDqRn;
  ControlBinop cBinop_JmRjXbmc;
  ControlBinop cBinop_tkMCzh4q;
  ControlBinop cBinop_MgNdjdBF;
  ControlBinop cBinop_a00iGGhj;
  ControlBinop cBinop_QDZYi7ql;
  ControlBinop cBinop_arv6taHI;
  ControlBinop cBinop_uNP7rWZx;
  ControlSlice cSlice_DfcbhiHe;
  ControlBinop cBinop_53kk02Ay;
  ControlBinop cBinop_sp3QIAbc;
  ControlBinop cBinop_4N6SktEw;
  ControlBinop cBinop_Bz7qHPTi;
  ControlBinop cBinop_HjgLSHyf;
  ControlBinop cBinop_MYEPFcRK;
  SignalVarf sVarf_uGfFQ7KK;
  SignalVarf sVarf_HBUbZLMB;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
