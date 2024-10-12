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
#include "HvControlVar.h"
#include "HvControlSlice.h"
#include "HvControlCast.h"
#include "HvMath.h"
#include "HvSignalPhasor.h"
#include "HvSignalVar.h"
#include "HvControlUnop.h"
#include "HvControlIf.h"
#include "HvControlBinop.h"

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

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void cSlice_njOVaWgK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_U8WB1UCQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_qqFMHHUq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_UwltQTy4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_8ikoS4qA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_OndsUuP3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_jMSjSu0i_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_vzIrLOOq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_iq6xRPv9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_8UDuVlHm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_LDXrXIyF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_vy6gKbVj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_J1oRIgoQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Aa2NERkf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_IHSnVLrZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_lKBRCHN9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ZNhKfnnj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_UFJZQ7z1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_wYsVwh6b_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Iw69LZOW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_cKeapj8V_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_SVRKwMyT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_NicfM6aA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_K5yeKJGb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_VxWDOwW0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_LTIGsoul_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_2RX9G84t_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_U81NxalZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_h1z921W9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_he1yGEbV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_hFAMCEE0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_kXIG7DiZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_lLAbQPuU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_Xv1UB8Ja_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_48mPeFov_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_XDzM0tYa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_foWZ5hiU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_5BpTCnZB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_Z4HmQZNe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_hXwfGsZP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_YXzjDoTv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_fKZ4NFJR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_glm8y1HH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_lMYdWNGZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_x602hRfg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_7C9DVNrF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_BUWOvw7V_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_lPQaiRUN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ulvY1sG3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Tg5b3aIt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_bd8MGeY3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_UWaXKsab_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_caJit7ZZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_7ZIHwyOP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Q5zHsRDk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Yfgzc8jN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_qn8Yx7ve_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_5JwjQYK5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_JAxu8FN0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_N5wVmH5e_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_LtqTUSaS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ErE8pj7i_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_qhCSaerc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_hDHgTjQp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_jpvSln7f_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_2wKdfXvc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_KkU5cQID_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_zrnerRPB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_BtT4A8kl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_QfSzJDHW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Uq3DnYI7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_IMjyIwUm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_qAIU69XY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_x3TVx2fE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_R11TuY2r_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_4YsXndkp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_CAEGMdpq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_kzlWM12n_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_ep6i56GT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_PVq143Av_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_HTFSp23A_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_ld0oD4Ki_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_4ARHdTIQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_nVkY4asf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_6g1OyEV6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_1OAHz7Wk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_pprPcucK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_qBGWHvzM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_kRQejlyA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_mw6CNlzm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_i2nFvnTB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_trl8ek5k_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_ahSFRJkv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_nlXBLGwp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_LBL2w8ST_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_xxaSKGxu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ej1QzzP2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_DafZuTGZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_jIjrI1fO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_FQQk43Xc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_SDytO6IM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_yOTUEeGx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_WosmBbHC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_HCnD54FH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_W7m5uEIo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_5qBBKbg0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_vBwRv8U6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_smOb0Z0Z_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_8II9fJni_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_vw4UKnFb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_h6TY7OQT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_V0jEmo2s_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_i7ByvO0e_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_B5F13IP5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_4GsbWKpy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_cuZUNL06_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_S54S82bP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_pXSisbR6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_x5weB8GE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_D3F2E1E0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_VWlwmaFh_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalPhasor sPhasor_5CRlF2CW;
  SignalPhasor sPhasor_10gjXKQw;
  ControlSlice cSlice_njOVaWgK;
  ControlSlice cSlice_U8WB1UCQ;
  ControlSlice cSlice_qqFMHHUq;
  ControlVar cVar_UwltQTy4;
  ControlIf cIf_jMSjSu0i;
  ControlBinop cBinop_vzIrLOOq;
  ControlIf cIf_iq6xRPv9;
  ControlIf cIf_8UDuVlHm;
  ControlIf cIf_LDXrXIyF;
  ControlIf cIf_vy6gKbVj;
  ControlBinop cBinop_J1oRIgoQ;
  ControlBinop cBinop_Aa2NERkf;
  ControlBinop cBinop_IHSnVLrZ;
  ControlBinop cBinop_Iw69LZOW;
  ControlBinop cBinop_cKeapj8V;
  ControlBinop cBinop_SVRKwMyT;
  ControlBinop cBinop_NicfM6aA;
  ControlBinop cBinop_K5yeKJGb;
  ControlVar cVar_LTIGsoul;
  ControlIf cIf_h1z921W9;
  ControlBinop cBinop_he1yGEbV;
  ControlVar cVar_hFAMCEE0;
  ControlIf cIf_Xv1UB8Ja;
  ControlBinop cBinop_48mPeFov;
  ControlSlice cSlice_XDzM0tYa;
  ControlSlice cSlice_foWZ5hiU;
  ControlSlice cSlice_5BpTCnZB;
  ControlIf cIf_Z4HmQZNe;
  ControlIf cIf_hXwfGsZP;
  ControlIf cIf_YXzjDoTv;
  ControlIf cIf_fKZ4NFJR;
  ControlIf cIf_glm8y1HH;
  ControlIf cIf_lMYdWNGZ;
  ControlIf cIf_x602hRfg;
  ControlIf cIf_7C9DVNrF;
  ControlIf cIf_BUWOvw7V;
  ControlVar cVar_lPQaiRUN;
  ControlBinop cBinop_ulvY1sG3;
  ControlBinop cBinop_Tg5b3aIt;
  ControlBinop cBinop_bd8MGeY3;
  ControlBinop cBinop_7ZIHwyOP;
  ControlBinop cBinop_Q5zHsRDk;
  ControlBinop cBinop_LtqTUSaS;
  ControlBinop cBinop_ErE8pj7i;
  ControlVar cVar_IMjyIwUm;
  ControlIf cIf_R11TuY2r;
  ControlBinop cBinop_4YsXndkp;
  ControlVar cVar_CAEGMdpq;
  ControlIf cIf_PVq143Av;
  ControlBinop cBinop_HTFSp23A;
  ControlSlice cSlice_ld0oD4Ki;
  ControlSlice cSlice_4ARHdTIQ;
  ControlSlice cSlice_nVkY4asf;
  ControlIf cIf_6g1OyEV6;
  ControlIf cIf_1OAHz7Wk;
  ControlIf cIf_pprPcucK;
  ControlIf cIf_qBGWHvzM;
  ControlIf cIf_kRQejlyA;
  ControlIf cIf_mw6CNlzm;
  ControlIf cIf_i2nFvnTB;
  ControlIf cIf_trl8ek5k;
  ControlIf cIf_ahSFRJkv;
  ControlVar cVar_nlXBLGwp;
  ControlBinop cBinop_LBL2w8ST;
  ControlBinop cBinop_xxaSKGxu;
  ControlBinop cBinop_ej1QzzP2;
  ControlBinop cBinop_FQQk43Xc;
  ControlBinop cBinop_SDytO6IM;
  ControlBinop cBinop_vBwRv8U6;
  ControlBinop cBinop_smOb0Z0Z;
  SignalVarf sVarf_mR3DdIoA;
  ControlBinop cBinop_pXSisbR6;
  SignalVarf sVarf_mAfI6IWY;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
