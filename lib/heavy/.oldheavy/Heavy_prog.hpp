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
#include "HvMath.h"
#include "HvControlSlice.h"
#include "HvSignalPhasor.h"
#include "HvControlDelay.h"
#include "HvSignalRPole.h"
#include "HvControlUnop.h"
#include "HvControlRandom.h"
#include "HvSignalSamphold.h"
#include "HvSignalDel1.h"
#include "HvControlSystem.h"
#include "HvControlBinop.h"
#include "HvControlVar.h"
#include "HvSignalVar.h"
#include "HvControlCast.h"
#include "HvSignalLine.h"

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
  static void cSwitchcase_8YFPA97X_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_s1Zt221g_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_GEGnMeCa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_tvCZZTgP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_zZV2wjrT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_UOuzwHvN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_8SvwEbHZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_RWBq5NCa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_mR5u45AH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_ykTCRb6V_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_IqSyBB4O_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_CR7rd9Xi_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_ocES4djs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_fyUsPkTw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_tkTKJMcW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_5UzuXYtY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_sBvM7K2O_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_9nHG4cVP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_9wu1LO6P_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_q5C11TLN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_eB0eRtOH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_zXHyF41m_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_l44xRp40_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_dkH6hYd9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_KK7h49tS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_hPie96KC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_s6fp4d3m_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_jnCUOsLu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_BDTzQxLX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_SwwQA5Gm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_l17Y4lPz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_NMNJz05W_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_MY2nwk8g_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_3wjErOIt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_65szakAT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_c8h43Y4Z_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_h9yWwxq9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_60UKaVhw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_ACwnzFLF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_7BP2ueh5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ZKsfLvmh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_4cGsCNG1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_P1ZQ1aQb_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_c4mmgH3q_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_gqrv309F_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_bsHyU1Id_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_yE6FEUB5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_JNkd4Qbj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_iHvqucs6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_F2Mazzn9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_lguPfJ7e_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_AIEcrSjI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_2QXSCS00_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_MOemsOGy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_M9Q8RVSQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ovyYpcZe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_ToOtg9Bo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_DzMvMLxV_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_AYcSWEXo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Cax03azf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_x806Xx0g_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_sbB2NyjN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_uc7VSxOC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_kgyqlZyV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_HRltZ8YF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_aERa10AO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_E5O7hueW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_uhSpEtXX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JONUKWEF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_wIsq3NZX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_xRAz92LM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ZLYskxSy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_gqz2LsnQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_gONpldeo_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_268WU45L_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_nw2B8U6o_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_9KZe9Aj6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_mXm2js6h_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_yFdO8pkD_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_YElchSvo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_aDESCeCw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_i5ZAhXNK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_2qPdCJ9N_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_sq6bN9gV_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_E8KWT3Rp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_FRHe1Kmt_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_r2ZwKE9j_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_Ihg9x8zC_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_29L1f6hf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_XWr7AFnr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_qND4rhiQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_jfaRQh43_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_l4R0GoJ2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_csTYtpO6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_7WygT5Es_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_jUjxu6iR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_8Wx5aoip_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_iZ9gEWFk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_QviOMl3H_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_UHQ7YAoa_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalRPole sRPole_nXF4B7Du;
  SignalDel1 sDel1_vtiP8yvJ;
  SignalLine sLine_vebMorzP;
  SignalPhasor sPhasor_x0w80lJD;
  SignalDel1 sDel1_ZeWG6SmZ;
  SignalSamphold sSamphold_Q7LSYzv3;
  SignalPhasor sPhasor_Odr3SXNg;
  SignalPhasor sPhasor_zUqRHGS2;
  SignalLine sLine_IAo3S42b;
  SignalRPole sRPole_SPbwTpQa;
  ControlBinop cBinop_s1Zt221g;
  ControlRandom cRandom_tvCZZTgP;
  ControlSlice cSlice_zZV2wjrT;
  SignalVari sVari_sPiFY57v;
  ControlVar cVar_8SvwEbHZ;
  ControlBinop cBinop_RWBq5NCa;
  ControlVar cVar_mR5u45AH;
  ControlVar cVar_ykTCRb6V;
  ControlVar cVar_IqSyBB4O;
  ControlBinop cBinop_jnCUOsLu;
  SignalVarf sVarf_RFYGyhKL;
  ControlDelay cDelay_3wjErOIt;
  ControlVar cVar_60UKaVhw;
  ControlBinop cBinop_7BP2ueh5;
  ControlBinop cBinop_ZKsfLvmh;
  ControlBinop cBinop_4cGsCNG1;
  ControlBinop cBinop_c4mmgH3q;
  ControlRandom cRandom_bsHyU1Id;
  ControlSlice cSlice_yE6FEUB5;
  ControlVar cVar_JNkd4Qbj;
  ControlBinop cBinop_lguPfJ7e;
  ControlBinop cBinop_AIEcrSjI;
  SignalVarf sVarf_w3WZP06U;
  ControlBinop cBinop_MOemsOGy;
  ControlBinop cBinop_M9Q8RVSQ;
  ControlBinop cBinop_ovyYpcZe;
  SignalVarf sVarf_j5FkzeJH;
  ControlVar cVar_ToOtg9Bo;
  ControlBinop cBinop_Cax03azf;
  ControlBinop cBinop_x806Xx0g;
  SignalVarf sVarf_XXPgzbIU;
  ControlVar cVar_sbB2NyjN;
  ControlBinop cBinop_HRltZ8YF;
  ControlBinop cBinop_aERa10AO;
  ControlBinop cBinop_uhSpEtXX;
  ControlBinop cBinop_JONUKWEF;
  ControlBinop cBinop_wIsq3NZX;
  ControlBinop cBinop_xRAz92LM;
  ControlBinop cBinop_ZLYskxSy;
  SignalVarf sVarf_DyNX63Ed;
  ControlVar cVar_gqz2LsnQ;
  ControlBinop cBinop_268WU45L;
  ControlRandom cRandom_9KZe9Aj6;
  ControlSlice cSlice_mXm2js6h;
  ControlBinop cBinop_YElchSvo;
  ControlRandom cRandom_i5ZAhXNK;
  ControlSlice cSlice_2qPdCJ9N;
  ControlBinop cBinop_E8KWT3Rp;
  ControlRandom cRandom_r2ZwKE9j;
  ControlSlice cSlice_Ihg9x8zC;
  ControlBinop cBinop_29L1f6hf;
  ControlBinop cBinop_XWr7AFnr;
  ControlBinop cBinop_jUjxu6iR;
  ControlBinop cBinop_8Wx5aoip;
  ControlBinop cBinop_iZ9gEWFk;
  SignalVarf sVarf_klnruZOo;
  SignalVarf sVarf_32e2ndy4;
  SignalVarf sVarf_TBcWCDe4;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
