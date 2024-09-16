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

#include "Heavy_prog.hpp"

#include <new>

#define Context(_c) static_cast<Heavy_prog *>(_c)


/*
 * C Functions
 */

extern "C" {
  HV_EXPORT HeavyContextInterface *hv_prog_new(double sampleRate) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_prog));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_prog(sampleRate);
    return Context(ptr);
  }

  HV_EXPORT HeavyContextInterface *hv_prog_new_with_options(double sampleRate,
      int poolKb, int inQueueKb, int outQueueKb) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_prog));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_prog(sampleRate, poolKb, inQueueKb, outQueueKb);
    return Context(ptr);
  }

  HV_EXPORT void hv_prog_free(HeavyContextInterface *instance) {
    // call destructor
    Context(instance)->~Heavy_prog();
    // free memory
    hv_free(instance);
  }
} // extern "C"







/*
 * Class Functions
 */

Heavy_prog::Heavy_prog(double sampleRate, int poolKb, int inQueueKb, int outQueueKb)
    : HeavyContext(sampleRate, poolKb, inQueueKb, outQueueKb) {
  numBytes += sRPole_init(&sRPole_nXF4B7Du);
  numBytes += sDel1_init(&sDel1_vtiP8yvJ);
  numBytes += sLine_init(&sLine_vebMorzP);
  numBytes += sPhasor_k_init(&sPhasor_x0w80lJD, 0.0f, sampleRate);
  numBytes += sDel1_init(&sDel1_ZeWG6SmZ);
  numBytes += sSamphold_init(&sSamphold_Q7LSYzv3);
  numBytes += sPhasor_init(&sPhasor_Odr3SXNg, sampleRate);
  numBytes += sPhasor_init(&sPhasor_zUqRHGS2, sampleRate);
  numBytes += sLine_init(&sLine_IAo3S42b);
  numBytes += sRPole_init(&sRPole_SPbwTpQa);
  numBytes += cRandom_init(&cRandom_tvCZZTgP, 433999533);
  numBytes += cSlice_init(&cSlice_zZV2wjrT, 1, 1);
  numBytes += sVari_init(&sVari_sPiFY57v, 0, 0, false);
  numBytes += cVar_init_f(&cVar_8SvwEbHZ, 1.0f);
  numBytes += cVar_init_f(&cVar_mR5u45AH, 0.0f);
  numBytes += cVar_init_f(&cVar_ykTCRb6V, 0.0f);
  numBytes += cVar_init_f(&cVar_IqSyBB4O, 0.0f);
  numBytes += cBinop_init(&cBinop_jnCUOsLu, 29.0f); // __div
  numBytes += sVarf_init(&sVarf_RFYGyhKL, 0.0f, 0.0f, false);
  numBytes += cDelay_init(this, &cDelay_3wjErOIt, 0.0f);
  numBytes += cVar_init_f(&cVar_60UKaVhw, 240.0f);
  numBytes += cBinop_init(&cBinop_4cGsCNG1, 0.0f); // __mul
  numBytes += cRandom_init(&cRandom_bsHyU1Id, -1332851572);
  numBytes += cSlice_init(&cSlice_yE6FEUB5, 1, 1);
  numBytes += cVar_init_f(&cVar_JNkd4Qbj, 22050.0f);
  numBytes += cBinop_init(&cBinop_lguPfJ7e, 0.0f); // __mul
  numBytes += sVarf_init(&sVarf_w3WZP06U, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_j5FkzeJH, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_ToOtg9Bo, 0.0f);
  numBytes += sVarf_init(&sVarf_XXPgzbIU, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_sbB2NyjN, 8000.0f);
  numBytes += cBinop_init(&cBinop_ZLYskxSy, 0.0f); // __div
  numBytes += sVarf_init(&sVarf_DyNX63Ed, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_gqz2LsnQ, 0.0f);
  numBytes += cRandom_init(&cRandom_9KZe9Aj6, -1164729606);
  numBytes += cSlice_init(&cSlice_mXm2js6h, 1, 1);
  numBytes += cRandom_init(&cRandom_i5ZAhXNK, -521753349);
  numBytes += cSlice_init(&cSlice_2qPdCJ9N, 1, 1);
  numBytes += cRandom_init(&cRandom_r2ZwKE9j, -834631576);
  numBytes += cSlice_init(&cSlice_Ihg9x8zC, 1, 1);
  numBytes += cBinop_init(&cBinop_iZ9gEWFk, 0.0f); // __mul
  numBytes += sVarf_init(&sVarf_klnruZOo, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_32e2ndy4, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_TBcWCDe4, 0.0f, 0.0f, false);
  
  // schedule a message to trigger all loadbangs via the __hv_init receiver
  scheduleMessageForReceiver(0xCE5CC65B, msg_initWithBang(HV_MESSAGE_ON_STACK(1), 0));
}

Heavy_prog::~Heavy_prog() {
  // nothing to free
}

HvTable *Heavy_prog::getTableForHash(hv_uint32_t tableHash) {
  return nullptr;
}

void Heavy_prog::scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_UHQ7YAoa_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_prog::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      default: {
        info->name = "invalid parameter index";
        info->hash = 0;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 0.0f;
        info->defaultVal = 0.0f;
        break;
      }
    }
  }
  return 0;
}



/*
 * Send Function Implementations
 */


void Heavy_prog::cSwitchcase_8YFPA97X_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_zZV2wjrT, 0, m, &cSlice_zZV2wjrT_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_tvCZZTgP, 0, m, &cRandom_tvCZZTgP_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_s1Zt221g_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_GEGnMeCa_sendMessage);
}

void Heavy_prog::cUnop_GEGnMeCa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_UOuzwHvN_sendMessage(_c, 0, m);
}

void Heavy_prog::cRandom_tvCZZTgP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8388610.0f, 0, m, &cBinop_s1Zt221g_sendMessage);
}

void Heavy_prog::cSlice_zZV2wjrT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_tvCZZTgP, 1, m, &cRandom_tvCZZTgP_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cMsg_UOuzwHvN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 1.0f);
  sVari_onMessage(_c, &Context(_c)->sVari_sPiFY57v, m);
}

void Heavy_prog::cVar_8SvwEbHZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_RWBq5NCa_sendMessage);
  cSwitchcase_MY2nwk8g_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cBinop_RWBq5NCa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_8SvwEbHZ, 1, m, &cVar_8SvwEbHZ_sendMessage);
}

void Heavy_prog::cVar_mR5u45AH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_CR7rd9Xi_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cVar_ykTCRb6V_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 16.0f, 0, m, &cBinop_29L1f6hf_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 30.0f, 0, m, &cBinop_XWr7AFnr_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_iZ9gEWFk, HV_BINOP_MULTIPLY, 1, m, &cBinop_iZ9gEWFk_sendMessage);
}

void Heavy_prog::cVar_IqSyBB4O_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_x0w80lJD, 0, m);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 16.0f, 0, m, &cBinop_jUjxu6iR_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_iZ9gEWFk, HV_BINOP_MULTIPLY, 0, m, &cBinop_iZ9gEWFk_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_32e2ndy4, m);
}

void Heavy_prog::cSwitchcase_CR7rd9Xi_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_ocES4djs_sendMessage);
      break;
    }
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_fyUsPkTw_sendMessage);
      break;
    }
    case 0x40000000: { // "2.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_tkTKJMcW_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_ocES4djs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_hPie96KC_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_dkH6hYd9_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_KK7h49tS_sendMessage);
}

void Heavy_prog::cCast_fyUsPkTw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_eB0eRtOH_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_zXHyF41m_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_l44xRp40_sendMessage);
}

void Heavy_prog::cCast_tkTKJMcW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_9wu1LO6P_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_q5C11TLN_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_9nHG4cVP_sendMessage);
}

void Heavy_prog::cCast_5UzuXYtY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_jnCUOsLu, HV_BINOP_DIVIDE, 0, m, &cBinop_jnCUOsLu_sendMessage);
}

void Heavy_prog::cCast_sBvM7K2O_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_s6fp4d3m_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_9nHG4cVP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_BDTzQxLX_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_9wu1LO6P_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_q5C11TLN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_SwwQA5Gm_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_eB0eRtOH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_zXHyF41m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_l17Y4lPz_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_l44xRp40_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_BDTzQxLX_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_dkH6hYd9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_NMNJz05W_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_KK7h49tS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_BDTzQxLX_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_hPie96KC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cMsg_s6fp4d3m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setFloat(m, 1, 0.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_IAo3S42b, 0, m, NULL);
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setElementToFrom(m, 1, n, 0);
  sLine_onMessage(_c, &Context(_c)->sLine_IAo3S42b, 0, m, NULL);
}

void Heavy_prog::cBinop_jnCUOsLu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_RFYGyhKL, m);
}

void Heavy_prog::cMsg_BDTzQxLX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 110.0f);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_sBvM7K2O_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5UzuXYtY_sendMessage);
}

void Heavy_prog::cMsg_SwwQA5Gm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 10.1f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_jnCUOsLu, HV_BINOP_DIVIDE, 1, m, &cBinop_jnCUOsLu_sendMessage);
}

void Heavy_prog::cMsg_l17Y4lPz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 20.9f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_jnCUOsLu, HV_BINOP_DIVIDE, 1, m, &cBinop_jnCUOsLu_sendMessage);
}

void Heavy_prog::cMsg_NMNJz05W_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 5.5f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_jnCUOsLu, HV_BINOP_DIVIDE, 1, m, &cBinop_jnCUOsLu_sendMessage);
}

void Heavy_prog::cSwitchcase_MY2nwk8g_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cMsg_ACwnzFLF_sendMessage(_c, 0, m);
      break;
    }
    case 0x7A5B032D: { // "stop"
      cMsg_ACwnzFLF_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_65szakAT_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_3wjErOIt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_3wjErOIt, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_3wjErOIt, 0, m, &cDelay_3wjErOIt_sendMessage);
  cSwitchcase_P1ZQ1aQb_onMessage(_c, NULL, 0, m, NULL);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_l4R0GoJ2_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_csTYtpO6_sendMessage);
  cSwitchcase_gONpldeo_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_yFdO8pkD_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cCast_65szakAT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_ACwnzFLF_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_3wjErOIt, 0, m, &cDelay_3wjErOIt_sendMessage);
  cSwitchcase_P1ZQ1aQb_onMessage(_c, NULL, 0, m, NULL);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_l4R0GoJ2_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_csTYtpO6_sendMessage);
  cSwitchcase_gONpldeo_onMessage(_c, NULL, 0, m, NULL);
  cSwitchcase_yFdO8pkD_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_c8h43Y4Z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_h9yWwxq9_sendMessage);
}

void Heavy_prog::cSystem_h9yWwxq9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_ZKsfLvmh_sendMessage);
}

void Heavy_prog::cVar_60UKaVhw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4cGsCNG1, HV_BINOP_MULTIPLY, 0, m, &cBinop_4cGsCNG1_sendMessage);
}

void Heavy_prog::cMsg_ACwnzFLF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_3wjErOIt, 0, m, &cDelay_3wjErOIt_sendMessage);
}

void Heavy_prog::cBinop_7BP2ueh5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_3wjErOIt, 2, m, &cDelay_3wjErOIt_sendMessage);
}

void Heavy_prog::cBinop_ZKsfLvmh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4cGsCNG1, HV_BINOP_MULTIPLY, 1, m, &cBinop_4cGsCNG1_sendMessage);
}

void Heavy_prog::cBinop_4cGsCNG1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_7BP2ueh5_sendMessage);
}

void Heavy_prog::cSwitchcase_P1ZQ1aQb_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_yE6FEUB5, 0, m, &cSlice_yE6FEUB5_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_bsHyU1Id, 0, m, &cRandom_bsHyU1Id_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_c4mmgH3q_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_gqrv309F_sendMessage);
}

void Heavy_prog::cUnop_gqrv309F_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_IqSyBB4O, 0, m, &cVar_IqSyBB4O_sendMessage);
}

void Heavy_prog::cRandom_bsHyU1Id_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_c4mmgH3q_sendMessage);
}

void Heavy_prog::cSlice_yE6FEUB5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_bsHyU1Id, 1, m, &cRandom_bsHyU1Id_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_JNkd4Qbj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_lguPfJ7e, HV_BINOP_MULTIPLY, 0, m, &cBinop_lguPfJ7e_sendMessage);
}

void Heavy_prog::cMsg_iHvqucs6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_F2Mazzn9_sendMessage);
}

void Heavy_prog::cSystem_F2Mazzn9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_2QXSCS00_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_lguPfJ7e_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_MOemsOGy_sendMessage);
}

void Heavy_prog::cBinop_AIEcrSjI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_lguPfJ7e, HV_BINOP_MULTIPLY, 1, m, &cBinop_lguPfJ7e_sendMessage);
}

void Heavy_prog::cMsg_2QXSCS00_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 6.28319f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_AIEcrSjI_sendMessage);
}

void Heavy_prog::cBinop_MOemsOGy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_M9Q8RVSQ_sendMessage);
}

void Heavy_prog::cBinop_M9Q8RVSQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 1.0f, 0, m, &cBinop_ovyYpcZe_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_j5FkzeJH, m);
}

void Heavy_prog::cBinop_ovyYpcZe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_w3WZP06U, m);
}

void Heavy_prog::cVar_ToOtg9Bo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_JNkd4Qbj, 0, m, &cVar_JNkd4Qbj_sendMessage);
  cMsg_QviOMl3H_sendMessage(_c, 0, m);
}

void Heavy_prog::cSwitchcase_DzMvMLxV_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_AYcSWEXo_sendMessage);
      break;
    }
    default: {
      cMsg_jfaRQh43_sendMessage(_c, 0, m);
      break;
    }
  }
}

void Heavy_prog::cCast_AYcSWEXo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_7WygT5Es_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_Cax03azf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.0f, 0, m, &cBinop_x806Xx0g_sendMessage);
}

void Heavy_prog::cBinop_x806Xx0g_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_HRltZ8YF_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -1.0f, 0, m, &cBinop_JONUKWEF_sendMessage);
}

void Heavy_prog::cVar_sbB2NyjN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_wIsq3NZX_sendMessage);
}

void Heavy_prog::cMsg_uc7VSxOC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_kgyqlZyV_sendMessage);
}

void Heavy_prog::cSystem_kgyqlZyV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ZLYskxSy, HV_BINOP_DIVIDE, 1, m, &cBinop_ZLYskxSy_sendMessage);
}

void Heavy_prog::cBinop_HRltZ8YF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_aERa10AO_sendMessage);
}

void Heavy_prog::cBinop_aERa10AO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_DyNX63Ed, m);
}

void Heavy_prog::cMsg_E5O7hueW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_uhSpEtXX_sendMessage);
}

void Heavy_prog::cBinop_uhSpEtXX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 1.0f, 0, m, &cBinop_Cax03azf_sendMessage);
}

void Heavy_prog::cBinop_JONUKWEF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_XXPgzbIU, m);
}

void Heavy_prog::cBinop_wIsq3NZX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 6.28319f, 0, m, &cBinop_xRAz92LM_sendMessage);
}

void Heavy_prog::cBinop_xRAz92LM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ZLYskxSy, HV_BINOP_DIVIDE, 0, m, &cBinop_ZLYskxSy_sendMessage);
}

void Heavy_prog::cBinop_ZLYskxSy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_E5O7hueW_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_gqz2LsnQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_sbB2NyjN, 0, m, &cVar_sbB2NyjN_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_JNkd4Qbj, 0, m, &cVar_JNkd4Qbj_sendMessage);
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_x0w80lJD, 1, m);
}

void Heavy_prog::cSwitchcase_gONpldeo_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_mXm2js6h, 0, m, &cSlice_mXm2js6h_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_9KZe9Aj6, 0, m, &cRandom_9KZe9Aj6_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_268WU45L_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_nw2B8U6o_sendMessage);
}

void Heavy_prog::cUnop_nw2B8U6o_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_mR5u45AH, 0, m, &cVar_mR5u45AH_sendMessage);
}

void Heavy_prog::cRandom_9KZe9Aj6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 4.0f, 0, m, &cBinop_268WU45L_sendMessage);
}

void Heavy_prog::cSlice_mXm2js6h_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_9KZe9Aj6, 1, m, &cRandom_9KZe9Aj6_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSwitchcase_yFdO8pkD_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_2qPdCJ9N, 0, m, &cSlice_2qPdCJ9N_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_i5ZAhXNK, 0, m, &cRandom_i5ZAhXNK_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_YElchSvo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_aDESCeCw_sendMessage);
}

void Heavy_prog::cUnop_aDESCeCw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_ykTCRb6V, 0, m, &cVar_ykTCRb6V_sendMessage);
}

void Heavy_prog::cRandom_i5ZAhXNK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 120.0f, 0, m, &cBinop_YElchSvo_sendMessage);
}

void Heavy_prog::cSlice_2qPdCJ9N_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_i5ZAhXNK, 1, m, &cRandom_i5ZAhXNK_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSwitchcase_sq6bN9gV_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_Ihg9x8zC, 0, m, &cSlice_Ihg9x8zC_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_r2ZwKE9j, 0, m, &cRandom_r2ZwKE9j_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cBinop_E8KWT3Rp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_FRHe1Kmt_sendMessage);
}

void Heavy_prog::cUnop_FRHe1Kmt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_DzMvMLxV_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cRandom_r2ZwKE9j_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_E8KWT3Rp_sendMessage);
}

void Heavy_prog::cSlice_Ihg9x8zC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_r2ZwKE9j, 1, m, &cRandom_r2ZwKE9j_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_29L1f6hf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_klnruZOo, m);
  sVarf_onMessage(_c, &Context(_c)->sVarf_32e2ndy4, m);
}

void Heavy_prog::cBinop_XWr7AFnr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_ToOtg9Bo, 0, m, &cVar_ToOtg9Bo_sendMessage);
}

void Heavy_prog::cMsg_qND4rhiQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_vebMorzP, 0, m, NULL);
}

void Heavy_prog::cMsg_jfaRQh43_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 20.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_vebMorzP, 0, m, NULL);
}

void Heavy_prog::cCast_l4R0GoJ2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_qND4rhiQ_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_csTYtpO6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_sq6bN9gV_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_7WygT5Es_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 80.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_vebMorzP, 0, m, NULL);
}

void Heavy_prog::cBinop_jUjxu6iR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_TBcWCDe4, m);
}

void Heavy_prog::cBinop_8Wx5aoip_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_gqz2LsnQ, 0, m, &cVar_gqz2LsnQ_sendMessage);
}

void Heavy_prog::cBinop_iZ9gEWFk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 110.0f, 0, m, &cBinop_8Wx5aoip_sendMessage);
}

void Heavy_prog::cMsg_QviOMl3H_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "seed");
  msg_setFloat(m, 1, 123.0f);
  cMsg_UOuzwHvN_sendMessage(_c, 0, m);
}

void Heavy_prog::cReceive_UHQ7YAoa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_8YFPA97X_onMessage(_c, NULL, 0, m, NULL);
  cMsg_c8h43Y4Z_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_60UKaVhw, 0, m, &cVar_60UKaVhw_sendMessage);
  cMsg_iHvqucs6_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_JNkd4Qbj, 0, m, &cVar_JNkd4Qbj_sendMessage);
  cMsg_uc7VSxOC_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_sbB2NyjN, 0, m, &cVar_sbB2NyjN_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_8SvwEbHZ, 0, m, &cVar_8SvwEbHZ_sendMessage);
}




/*
 * Context Process Implementation
 */

int Heavy_prog::process(float **inputBuffers, float **outputBuffers, int n) {
  while (hLp_hasData(&inQueue)) {
    hv_uint32_t numBytes = 0;
    ReceiverMessagePair *p = reinterpret_cast<ReceiverMessagePair *>(hLp_getReadBuffer(&inQueue, &numBytes));
    hv_assert(numBytes >= sizeof(ReceiverMessagePair));
    scheduleMessageForReceiver(p->receiverHash, &p->msg);
    hLp_consume(&inQueue);
  }

  sendBangToReceiver(0xDD21C0EB); // send to __hv_bang~ on next cycle
  const int n4 = n & ~HV_N_SIMD_MASK; // ensure that the block size is a multiple of HV_N_SIMD

  // temporary signal vars
  hv_bufferf_t Bf0, Bf1, Bf2, Bf3, Bf4, Bf5, Bf6, Bf7, Bf8, Bf9, Bf10;
  hv_bufferi_t Bi0, Bi1;

  // input and output vars
  hv_bufferf_t O0, O1;

  // declare and init the zero buffer
  hv_bufferf_t ZERO; __hv_zero_f(VOf(ZERO));

  hv_uint32_t nextBlock = blockStartTimestamp;
  for (int n = 0; n < n4; n += HV_N_SIMD) {

    // process all of the messages for this block
    nextBlock += HV_N_SIMD;
    while (mq_hasMessageBefore(&mq, nextBlock)) {
      MessageNode *const node = mq_peek(&mq);
      node->sendMessage(this, node->let, node->m);
      mq_pop(&mq);
    }

    

    // zero output buffers
    __hv_zero_f(VOf(O0));
    __hv_zero_f(VOf(O1));

    // process all signal functions
    __hv_varread_i(&sVari_sPiFY57v, VOi(Bi0));
    __hv_var_k_i(VOi(Bi1), 16807, 16807, 16807, 16807, 16807, 16807, 16807, 16807);
    __hv_mul_i(VIi(Bi0), VIi(Bi1), VOi(Bi1));
    __hv_cast_if(VIi(Bi1), VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f, 4.65661e-10f);
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_varwrite_i(&sVari_sPiFY57v, VIi(Bi1));
    __hv_varread_f(&sVarf_XXPgzbIU, VOf(Bf0));
    __hv_rpole_f(&sRPole_nXF4B7Du, VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf2), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_del1_f(&sDel1_vtiP8yvJ, VIf(Bf0), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf2), VOf(Bf2));
    __hv_sub_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_varread_f(&sVarf_DyNX63Ed, VOf(Bf0));
    __hv_mul_f(VIf(Bf2), VIf(Bf0), VOf(Bf0));
    __hv_line_f(&sLine_vebMorzP, VOf(Bf2));
    __hv_mul_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_var_k_f(VOf(Bf0), 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
    __hv_phasor_k_f(&sPhasor_x0w80lJD, VOf(Bf3));
    __hv_del1_f(&sDel1_ZeWG6SmZ, VIf(Bf3), VOf(Bf4));
    __hv_lt_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_samphold_f(&sSamphold_Q7LSYzv3, VIf(Bf1), VIf(Bf4), VOf(Bf4));
    __hv_add_f(VIf(Bf4), VIf(Bf2), VOf(Bf5));
    __hv_varread_f(&sVarf_klnruZOo, VOf(Bf6));
    __hv_mul_f(VIf(Bf5), VIf(Bf6), VOf(Bf6));
    __hv_add_f(VIf(ZERO), VIf(Bf6), VOf(Bf5));
    __hv_var_k_f(VOf(Bf7), 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f, 256.0f);
    __hv_mul_f(VIf(Bf3), VIf(Bf7), VOf(Bf7));
    __hv_mul_f(VIf(Bf5), VIf(Bf7), VOf(Bf7));
    __hv_phasor_f(&sPhasor_Odr3SXNg, VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf5), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf7), VIf(Bf5), VOf(Bf5));
    __hv_abs_f(VIf(Bf5), VOf(Bf5));
    __hv_var_k_f(VOf(Bf7), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf5), VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf5), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf7), VIf(Bf5), VOf(Bf5));
    __hv_mul_f(VIf(Bf5), VIf(Bf5), VOf(Bf7));
    __hv_mul_f(VIf(Bf5), VIf(Bf7), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf8), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf9), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf3), VIf(Bf9), VIf(Bf5), VOf(Bf5));
    __hv_fma_f(VIf(Bf7), VIf(Bf8), VIf(Bf5), VOf(Bf5));
    __hv_add_f(VIf(Bf5), VIf(Bf6), VOf(Bf6));
    __hv_varread_f(&sVarf_32e2ndy4, VOf(Bf8));
    __hv_fma_f(VIf(Bf6), VIf(Bf8), VIf(Bf4), VOf(Bf4));
    __hv_phasor_f(&sPhasor_zUqRHGS2, VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf8), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf4), VIf(Bf8), VOf(Bf8));
    __hv_abs_f(VIf(Bf8), VOf(Bf8));
    __hv_var_k_f(VOf(Bf4), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf8), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf8), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf4), VIf(Bf8), VOf(Bf8));
    __hv_mul_f(VIf(Bf8), VIf(Bf8), VOf(Bf4));
    __hv_mul_f(VIf(Bf8), VIf(Bf4), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf7), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf9), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf6), VIf(Bf9), VIf(Bf8), VOf(Bf8));
    __hv_fma_f(VIf(Bf4), VIf(Bf7), VIf(Bf8), VOf(Bf8));
    __hv_line_f(&sLine_IAo3S42b, VOf(Bf7));
    __hv_varread_f(&sVarf_RFYGyhKL, VOf(Bf4));
    __hv_mul_f(VIf(Bf7), VIf(Bf4), VOf(Bf4));
    __hv_mul_f(VIf(Bf4), VIf(Bf4), VOf(Bf4));
    __hv_mul_f(VIf(Bf4), VIf(Bf7), VOf(Bf4));
    __hv_floor_f(VIf(Bf4), VOf(Bf9));
    __hv_sub_f(VIf(Bf4), VIf(Bf9), VOf(Bf9));
    __hv_var_k_f(VOf(Bf4), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf9), VIf(Bf4), VOf(Bf4));
    __hv_abs_f(VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf9), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf4), VIf(Bf9), VOf(Bf9));
    __hv_var_k_f(VOf(Bf4), 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f);
    __hv_mul_f(VIf(Bf9), VIf(Bf4), VOf(Bf4));
    __hv_mul_f(VIf(Bf4), VIf(Bf4), VOf(Bf9));
    __hv_mul_f(VIf(Bf4), VIf(Bf9), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf9), VOf(Bf9));
    __hv_var_k_f(VOf(Bf3), 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f);
    __hv_var_k_f(VOf(Bf10), 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f);
    __hv_mul_f(VIf(Bf6), VIf(Bf10), VOf(Bf10));
    __hv_sub_f(VIf(Bf4), VIf(Bf10), VOf(Bf10));
    __hv_fma_f(VIf(Bf9), VIf(Bf3), VIf(Bf10), VOf(Bf10));
    __hv_mul_f(VIf(Bf10), VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf10), 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f);
    __hv_mul_f(VIf(Bf7), VIf(Bf10), VOf(Bf10));
    __hv_varread_f(&sVarf_TBcWCDe4, VOf(Bf7));
    __hv_mul_f(VIf(Bf1), VIf(Bf7), VOf(Bf7));
    __hv_varread_f(&sVarf_j5FkzeJH, VOf(Bf1));
    __hv_mul_f(VIf(Bf7), VIf(Bf1), VOf(Bf1));
    __hv_varread_f(&sVarf_w3WZP06U, VOf(Bf7));
    __hv_rpole_f(&sRPole_SPbwTpQa, VIf(Bf1), VIf(Bf7), VOf(Bf7));
    __hv_add_f(VIf(Bf10), VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf10), 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
    __hv_mul_f(VIf(Bf7), VIf(Bf10), VOf(Bf10));
    __hv_add_f(VIf(Bf8), VIf(Bf10), VOf(Bf7));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf7), VOf(Bf7));
    __hv_var_k_f(VOf(Bf0), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf7), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O1), VOf(O1));
    __hv_var_k_f(VOf(Bf0), 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f);
    __hv_add_f(VIf(Bf5), VIf(Bf10), VOf(Bf10));
    __hv_fma_f(VIf(Bf2), VIf(Bf0), VIf(Bf10), VOf(Bf10));
    __hv_var_k_f(VOf(Bf0), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf10), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O0), VOf(O0));

    // save output vars to output buffer
    __hv_store_f(outputBuffers[0]+n, VIf(O0));
    __hv_store_f(outputBuffers[1]+n, VIf(O1));
  }

  blockStartTimestamp = nextBlock;

  return n4; // return the number of frames processed

}

int Heavy_prog::processInline(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s)
  float **const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float **const bOut = reinterpret_cast<float **>(hv_alloca(2*sizeof(float *)));
  bOut[0] = outputBuffers+(0*n4);
  bOut[1] = outputBuffers+(1*n4);

  int n = process(bIn, bOut, n4);
  return n;
}

int Heavy_prog::processInlineInterleaved(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(n4 & ~HV_N_SIMD_MASK); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s), uninterleave
  float *const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float *const bOut = reinterpret_cast<float *>(hv_alloca(2*n4*sizeof(float)));

  int n = processInline(bIn, bOut, n4);

  // interleave the heavy output into the output buffer
  #if HV_SIMD_AVX
  for (int i = 0, j = 0; j < n4; j += 8, i += 16) {
    __m256 x = _mm256_load_ps(bOut+j);    // LLLLLLLL
    __m256 y = _mm256_load_ps(bOut+n4+j); // RRRRRRRR
    __m256 a = _mm256_unpacklo_ps(x, y);  // LRLRLRLR
    __m256 b = _mm256_unpackhi_ps(x, y);  // LRLRLRLR
    _mm256_store_ps(outputBuffers+i, a);
    _mm256_store_ps(outputBuffers+8+i, b);
  }
  #elif HV_SIMD_SSE
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    __m128 x = _mm_load_ps(bOut+j);    // LLLL
    __m128 y = _mm_load_ps(bOut+n4+j); // RRRR
    __m128 a = _mm_unpacklo_ps(x, y);  // LRLR
    __m128 b = _mm_unpackhi_ps(x, y);  // LRLR
    _mm_store_ps(outputBuffers+i, a);
    _mm_store_ps(outputBuffers+4+i, b);
  }
  #elif HV_SIMD_NEON
  // https://community.arm.com/groups/processors/blog/2012/03/13/coding-for-neon--part-5-rearranging-vectors
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    float32x4_t x = vld1q_f32(bOut+j);
    float32x4_t y = vld1q_f32(bOut+n4+j);
    float32x4x2_t z = {x, y};
    vst2q_f32(outputBuffers+i, z); // interleave and store
  }
  #else // HV_SIMD_NONE
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < n4; ++j) {
      outputBuffers[i+2*j] = bOut[i*n4+j];
    }
  }
  #endif

  return n;
}
