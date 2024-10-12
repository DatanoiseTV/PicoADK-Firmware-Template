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
#include "HvControlSlice.h"
#include "HvSignalVar.h"
#include "HvControlBinop.h"
#include "HvSignalPhasor.h"
#include "HvControlCast.h"
#include "HvMath.h"
#include "HvControlVar.h"
#include "HvControlUnop.h"
#include "HvControlIf.h"

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
  static void cSlice_7FDdK6T1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_maSKos7I_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_PUG57Jex_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_KUKY92vm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_Ndp0lOm6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_C9JUA2Al_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_IlOOHw50_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_J8X3jxKp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_rmECsuW2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_AigRLO9W_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_wdeNKQeu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_5nvH5JCe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_KZsFjOXd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_dxa0c7a9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_IGZfCQq7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ursitnSV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_eTb8hngH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_nJgAcTxP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_0tQ0xQ7a_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_IBKzrHiN_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_1q8jwjGL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_PjEL715h_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_KYjCCH1a_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_eKVm5J1z_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_a13eg3mz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_eogrP5YK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_fNi95GNr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_V9fFuZDQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_hLj0i5MA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_BdxVhBeM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_5FQydvFo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_5rq4xJ1w_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_em4jLeOM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_hpfMkxNs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_fSLlHbdK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_WaaNDK7e_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_0R5PYa4L_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_UPgDSKI5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_sXfJLH8N_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_8drHcHwl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_BljEUZR3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_zpI80Znz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_cz7p1Iv7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_CdYUDEUx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_v2GCU7nc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_3Uff9V5B_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cIf_FElZMpJf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_0b43KBEW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_j3MVbJW5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_q7eY7fjH_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_terlue8e_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_H0RAP57B_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_1WTAZX9W_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_lEGOlVWs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_YOrG7TpK_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_3s2Cno8l_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_M6GCr5zU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_ttYIzJgg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_jD1aoycG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_fcLKgzzz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_2KlGbzbc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Wq73laAg_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_vUsL5pOI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_jb1dJY9G_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_4FHgelvL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_LrnBkiud_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_SId5IZSd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_JzNZu9oz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_QZzOMGrb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_5jCk77pU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Yrq8jZWR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_POchai4X_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_bxCS8sSh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_uGIFXRGu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_Js0vSq3Z_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalPhasor sPhasor_H9u1rhpH;
  ControlSlice cSlice_7FDdK6T1;
  ControlSlice cSlice_maSKos7I;
  ControlSlice cSlice_PUG57Jex;
  ControlVar cVar_KUKY92vm;
  ControlIf cIf_IlOOHw50;
  ControlBinop cBinop_J8X3jxKp;
  ControlIf cIf_rmECsuW2;
  ControlIf cIf_AigRLO9W;
  ControlIf cIf_wdeNKQeu;
  ControlIf cIf_5nvH5JCe;
  ControlBinop cBinop_KZsFjOXd;
  ControlBinop cBinop_dxa0c7a9;
  ControlBinop cBinop_IGZfCQq7;
  ControlBinop cBinop_IBKzrHiN;
  ControlBinop cBinop_1q8jwjGL;
  ControlBinop cBinop_PjEL715h;
  ControlBinop cBinop_KYjCCH1a;
  ControlBinop cBinop_eKVm5J1z;
  ControlVar cVar_eogrP5YK;
  ControlIf cIf_hLj0i5MA;
  ControlBinop cBinop_BdxVhBeM;
  ControlVar cVar_5FQydvFo;
  ControlIf cIf_hpfMkxNs;
  ControlBinop cBinop_fSLlHbdK;
  ControlSlice cSlice_WaaNDK7e;
  ControlSlice cSlice_0R5PYa4L;
  ControlSlice cSlice_UPgDSKI5;
  ControlIf cIf_sXfJLH8N;
  ControlIf cIf_8drHcHwl;
  ControlIf cIf_BljEUZR3;
  ControlIf cIf_zpI80Znz;
  ControlIf cIf_cz7p1Iv7;
  ControlIf cIf_CdYUDEUx;
  ControlIf cIf_v2GCU7nc;
  ControlIf cIf_3Uff9V5B;
  ControlIf cIf_FElZMpJf;
  ControlVar cVar_0b43KBEW;
  ControlBinop cBinop_j3MVbJW5;
  ControlBinop cBinop_q7eY7fjH;
  ControlBinop cBinop_terlue8e;
  ControlBinop cBinop_lEGOlVWs;
  ControlBinop cBinop_YOrG7TpK;
  ControlBinop cBinop_2KlGbzbc;
  ControlBinop cBinop_Wq73laAg;
  SignalVarf sVarf_f6CQApO6;
  ControlBinop cBinop_POchai4X;
};

#endif // _HEAVY_CONTEXT_PROG_HPP_
