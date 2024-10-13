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
  numBytes += sPhasor_init(&sPhasor_8r9Gzpcz, sampleRate);
  numBytes += sLine_init(&sLine_EfgfSfdV);
  numBytes += sLine_init(&sLine_lfViwZwu);
  numBytes += sLine_init(&sLine_W7Xbi5S5);
  numBytes += sLine_init(&sLine_sqsDTgvq);
  numBytes += sLine_init(&sLine_Tx0TGfLu);
  numBytes += sBiquad_init(&sBiquad_s_MqBlhoR5);
  numBytes += sPhasor_init(&sPhasor_INtIFWLT, sampleRate);
  numBytes += sLine_init(&sLine_MOufmpiq);
  numBytes += sLine_init(&sLine_AHZMYFM8);
  numBytes += sLine_init(&sLine_OSGuqlCO);
  numBytes += sLine_init(&sLine_cSgka7VF);
  numBytes += sLine_init(&sLine_zGcTLAb4);
  numBytes += sBiquad_init(&sBiquad_s_gCeCZYC1);
  numBytes += cVar_init_f(&cVar_b1MHLKJQ, 0.0f);
  numBytes += sVarf_init(&sVarf_AFhXb2iL, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_xIeAUoAA, 0.0f);
  numBytes += sVarf_init(&sVarf_rg72z3nJ, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_55uGsKPW, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_cJAJU7yZ, 0.0f);
  numBytes += sVarf_init(&sVarf_qTWDqhju, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_bBkXyPVt, 0.0f);
  numBytes += cBinop_init(&cBinop_ipNxaB9i, 44100.0f); // __div
  numBytes += cBinop_init(&cBinop_YfrKjul2, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_iblGHuAd, 0.0f);
  numBytes += cVar_init_f(&cVar_p14vECrQ, 1000.0f);
  numBytes += cVar_init_f(&cVar_2hxr9pSY, 4.0f);
  numBytes += cBinop_init(&cBinop_wXpL7Lha, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_E0krvJiJ, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_nOZAkW0f, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_IgM69xDY, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_G1B7jfBh, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_b09mYft7, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_ovXrdvNy, 44100.0f); // __div
  numBytes += cBinop_init(&cBinop_LT3MVoYV, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_brbkR7aA, 0.0f);
  numBytes += cVar_init_f(&cVar_trSuFGlB, 1000.0f);
  numBytes += cVar_init_f(&cVar_2RilFZLD, 4.0f);
  numBytes += cBinop_init(&cBinop_e5WuT1Hu, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_4MgQBkQ1, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_c2SPWkuP, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_ZDh490Jg, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_BAL6umtX, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_P8OZtvdS, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_FDeTeUkm, 0.0f);
  numBytes += cSlice_init(&cSlice_8sJM8Vqs, 2, 1);
  numBytes += cSlice_init(&cSlice_uvVB9JWN, 1, 1);
  numBytes += cSlice_init(&cSlice_aVtOtDdW, 0, 1);
  numBytes += cVar_init_f(&cVar_3JPv7bcq, 0.0f);
  numBytes += cIf_init(&cIf_1kT39VZZ, false);
  numBytes += cIf_init(&cIf_KCeKetXN, false);
  numBytes += cIf_init(&cIf_TDDAYhi7, false);
  numBytes += cIf_init(&cIf_bqREttei, false);
  numBytes += cIf_init(&cIf_Cb8vZZnJ, false);
  numBytes += cBinop_init(&cBinop_Hh9DAixe, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_FJWptXfR, 1.0f); // __pow
  numBytes += cVar_init_f(&cVar_pkcTTgHR, 42.0f);
  numBytes += cIf_init(&cIf_y7UhwfMD, true);
  numBytes += cVar_init_f(&cVar_BdiVtWFA, 0.0f);
  numBytes += cIf_init(&cIf_8nDKFkdS, false);
  numBytes += cSlice_init(&cSlice_cY26xX6b, 2, 1);
  numBytes += cSlice_init(&cSlice_beD3LScr, 1, 1);
  numBytes += cSlice_init(&cSlice_0BzyKePF, 0, 1);
  numBytes += cIf_init(&cIf_cHUWc560, false);
  numBytes += cIf_init(&cIf_RqxnlCKS, false);
  numBytes += cIf_init(&cIf_omnTRxhK, false);
  numBytes += cIf_init(&cIf_7jAdJNx3, true);
  numBytes += cIf_init(&cIf_KzIKDkqD, true);
  numBytes += cIf_init(&cIf_dlklO1b8, true);
  numBytes += cIf_init(&cIf_c5lwRKa2, true);
  numBytes += cIf_init(&cIf_SUXaUi8J, false);
  numBytes += cIf_init(&cIf_yzWfyBqm, false);
  numBytes += cVar_init_f(&cVar_HpXknl5P, 0.0f);
  numBytes += cBinop_init(&cBinop_nHoBhO2B, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_B8arjK07, 0.0f); // __eq
  numBytes += cVar_init_f(&cVar_wuRMR9le, 43.0f);
  numBytes += cIf_init(&cIf_DALmR1vY, true);
  numBytes += cVar_init_f(&cVar_vXZZN7Wo, 0.0f);
  numBytes += cIf_init(&cIf_7p8993MK, false);
  numBytes += cSlice_init(&cSlice_9cShn2pW, 2, 1);
  numBytes += cSlice_init(&cSlice_nww5UdEW, 1, 1);
  numBytes += cSlice_init(&cSlice_r5W61ffA, 0, 1);
  numBytes += cIf_init(&cIf_MXJfx3O8, false);
  numBytes += cIf_init(&cIf_J9cTGCzO, false);
  numBytes += cIf_init(&cIf_bwU2VgGw, false);
  numBytes += cIf_init(&cIf_uh8beHpa, true);
  numBytes += cIf_init(&cIf_K1D1JO56, true);
  numBytes += cIf_init(&cIf_zaiWJWpD, true);
  numBytes += cIf_init(&cIf_E5pIRyr8, true);
  numBytes += cIf_init(&cIf_tKJwzIqi, false);
  numBytes += cIf_init(&cIf_USaaUumh, false);
  numBytes += cVar_init_f(&cVar_l6GM6Mzf, 0.0f);
  numBytes += cBinop_init(&cBinop_Phgf8WwC, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_RGapAWdH, 0.0f); // __eq
  numBytes += cVar_init_f(&cVar_Hg9kPjvt, 44.0f);
  numBytes += cIf_init(&cIf_bknadFO3, true);
  numBytes += cVar_init_f(&cVar_yeV5qANo, 0.0f);
  numBytes += cIf_init(&cIf_oJeYhaVK, false);
  numBytes += cSlice_init(&cSlice_9b1oKPAz, 2, 1);
  numBytes += cSlice_init(&cSlice_BhbzN3lY, 1, 1);
  numBytes += cSlice_init(&cSlice_LoH4l7Du, 0, 1);
  numBytes += cIf_init(&cIf_4fg45QhZ, false);
  numBytes += cIf_init(&cIf_9TwSE3l9, false);
  numBytes += cIf_init(&cIf_EC9nV2d6, false);
  numBytes += cIf_init(&cIf_K9wcvEmq, true);
  numBytes += cIf_init(&cIf_CTHQlZd3, true);
  numBytes += cIf_init(&cIf_uyJsdmNE, true);
  numBytes += cIf_init(&cIf_HCQkqk7n, true);
  numBytes += cIf_init(&cIf_5P7qD57V, false);
  numBytes += cIf_init(&cIf_BAEBVWF3, false);
  numBytes += cVar_init_f(&cVar_e7jY34PG, 0.0f);
  numBytes += cBinop_init(&cBinop_8eTeWi4z, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_6415d2ls, 0.0f); // __eq
  numBytes += cSlice_init(&cSlice_IxJCHTdk, 1, 1);
  numBytes += cSlice_init(&cSlice_1VB5q5bI, 0, 1);
  numBytes += cBinop_init(&cBinop_XMU3d1kd, 0.0f); // __sub
  numBytes += cBinop_init(&cBinop_rvjA5qjA, 0.0f); // __div
  numBytes += cIf_init(&cIf_oLoIHh0h, false);
  numBytes += cVar_init_f(&cVar_bNhD7Qh5, 1.0f);
  numBytes += cVar_init_f(&cVar_xuFLU1OF, 0.0f);
  numBytes += cVar_init_f(&cVar_s2qz4Wip, 0.0f);
  numBytes += cDelay_init(this, &cDelay_uMMHGtXd, 500.0f);
  numBytes += cIf_init(&cIf_1zSV3iaP, false);
  numBytes += cVar_init_f(&cVar_qN5mjKXS, 32.0f);
  numBytes += cVar_init_f(&cVar_FRH45GIQ, 0.0f);
  numBytes += cVar_init_f(&cVar_YXU6ke3k, 0.0f);
  numBytes += cDelay_init(this, &cDelay_RaYcPW50, 1.0f);
  numBytes += cVar_init_f(&cVar_SArVFfWE, 0.0f);
  numBytes += cDelay_init(this, &cDelay_zru4532F, 0.0f);
  numBytes += cVar_init_f(&cVar_nu048CLv, 20.0f);
  numBytes += cBinop_init(&cBinop_TUWUJLAa, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_wGcPHTZz, 0.0f);
  numBytes += cSlice_init(&cSlice_U1jLjdCp, 1, -1);
  numBytes += cSlice_init(&cSlice_sUlUYeNs, 1, -1);
  numBytes += cVar_init_f(&cVar_SJWgBMEp, 0.0f);
  numBytes += cVar_init_f(&cVar_gHq9nltM, 20.0f);
  numBytes += cVar_init_f(&cVar_E23hfvrT, 0.0f);
  numBytes += cVar_init_f(&cVar_rod0aeKq, 0.0f);
  numBytes += cVar_init_f(&cVar_WkPWrzFw, 0.0f);
  numBytes += cSlice_init(&cSlice_G0EFu0UW, 1, 1);
  numBytes += cSlice_init(&cSlice_6Za39v3y, 0, 1);
  numBytes += cBinop_init(&cBinop_4r6fcEBT, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_4GOoOJQ2, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_XgulPMUY, 0.0f); // __sub
  numBytes += cBinop_init(&cBinop_Xnc8lmg9, 0.0f); // __add
  numBytes += cBinop_init(&cBinop_lTf1xhFW, 20.0f); // __div
  numBytes += cBinop_init(&cBinop_ygcOjkrG, 0.0f); // __div
  numBytes += cBinop_init(&cBinop_jPneGUvo, 0.0f); // __add
  numBytes += cBinop_init(&cBinop_938vmUmW, 0.0f); // __sub
  numBytes += cSlice_init(&cSlice_hcqGbkXj, 1, -1);
  numBytes += cBinop_init(&cBinop_luqHfF5G, 2.0f); // __sub
  numBytes += cBinop_init(&cBinop_W4dtgSD9, 2.0f); // __add
  numBytes += sVarf_init(&sVarf_XUaSTwuF, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_SiNH5FLu, 0.0f, 0.0f, false);
  
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
    case 0x94DDDE92: { // 1062-alpha
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_UZ3q3013_sendMessage);
      break;
    }
    case 0x6F9D23C1: { // 1062-wcos
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_jLGrvod3_sendMessage);
      break;
    }
    case 0xD4FCB382: { // 1062-wsin
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_hEXxY5BZ_sendMessage);
      break;
    }
    case 0xEE0A93FE: { // 1103-alpha
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_RjUHK0UV_sendMessage);
      break;
    }
    case 0x729D4D39: { // 1103-wcos
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_ujFLffTo_sendMessage);
      break;
    }
    case 0x4068BFB3: { // 1103-wsin
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_1uYiVQSR_sendMessage);
      break;
    }
    case 0x41BE0F9C: { // __hv_ctlin
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_iY2aKgYk_sendMessage);
      break;
    }
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_ijIwaagV_sendMessage);
      break;
    }
    case 0x6FFF0BCF: { // __hv_midirealtimein
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_2yg3PMZB_sendMessage);
      break;
    }
    case 0x67E37CA3: { // __hv_notein
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_zC0sXkWV_sendMessage);
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


void Heavy_prog::cVar_b1MHLKJQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xr8p7Jz8_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_v61dL0a8_sendMessage);
}

void Heavy_prog::cVar_xIeAUoAA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_onMessage(_c, &Context(_c)->sPhasor_INtIFWLT, 1, m);
}

void Heavy_prog::cMsg_3CG8bgKq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_CTU9JZ3r_sendMessage);
}

void Heavy_prog::cSystem_CTU9JZ3r_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_rA169wL1_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_rA169wL1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_JNCfpXHH_sendMessage);
}

void Heavy_prog::cBinop_JNCfpXHH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_rg72z3nJ, m);
}

void Heavy_prog::cVar_cJAJU7yZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_onMessage(_c, &Context(_c)->sPhasor_8r9Gzpcz, 1, m);
}

void Heavy_prog::cMsg_B3whapdc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_YGQU8lrg_sendMessage);
}

void Heavy_prog::cSystem_YGQU8lrg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_ACofvsjJ_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_ACofvsjJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_0JAW4FK1_sendMessage);
}

void Heavy_prog::cBinop_0JAW4FK1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_qTWDqhju, m);
}

void Heavy_prog::cVar_bBkXyPVt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 127.0f, 0, m, &cBinop_0Btjt5FJ_sendMessage);
}

void Heavy_prog::cMsg_xIRbad3Y_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_EfgfSfdV, 0, m, NULL);
}

void Heavy_prog::cMsg_qDw6EBd8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_lfViwZwu, 0, m, NULL);
}

void Heavy_prog::cMsg_ZRBId9kl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_W7Xbi5S5, 0, m, NULL);
}

void Heavy_prog::cMsg_BNnlPypU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_sqsDTgvq, 0, m, NULL);
}

void Heavy_prog::cMsg_h7mZI4oM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_Tx0TGfLu, 0, m, NULL);
}

void Heavy_prog::cMsg_O9NfMaGc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_WmNnIB85_sendMessage);
}

void Heavy_prog::cSystem_WmNnIB85_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ipNxaB9i, HV_BINOP_DIVIDE, 1, m, &cBinop_ipNxaB9i_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_TaTRsX89_sendMessage);
}

void Heavy_prog::cUnop_VjWUnG9I_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_vEjyRdVE_sendMessage);
}

void Heavy_prog::cMsg_Td9LNGnf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cUnop_onMessage(_c, HV_UNOP_ATAN, m, &cUnop_VjWUnG9I_sendMessage);
}

void Heavy_prog::cBinop_vEjyRdVE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ipNxaB9i, HV_BINOP_DIVIDE, 0, m, &cBinop_ipNxaB9i_sendMessage);
}

void Heavy_prog::cCast_TaTRsX89_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_Td9LNGnf_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_ipNxaB9i_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_YfrKjul2, HV_BINOP_MULTIPLY, 1, m, &cBinop_YfrKjul2_sendMessage);
}

void Heavy_prog::cBinop_YfrKjul2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_h2nIFqoO_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_qBRVoP3w_sendMessage);
}

void Heavy_prog::cUnop_aNuuoAVO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_Mw6hBWnQ_sendMessage(_c, 0, m);
}

void Heavy_prog::cUnop_TLxpHWn4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_CaMPbp8r_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_PEeSVqgv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 20.0f, 0, m, &cBinop_xpCDRrdj_sendMessage);
}

void Heavy_prog::cBinop_xpCDRrdj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_YfrKjul2, HV_BINOP_MULTIPLY, 0, m, &cBinop_YfrKjul2_sendMessage);
}

void Heavy_prog::cBinop_oV1xRaEd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.1f, 0, m, &cBinop_Xwo9dV1F_sendMessage);
}

void Heavy_prog::cBinop_Xwo9dV1F_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_w7zCFVlF_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_C5qGIWp4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_E0krvJiJ, HV_BINOP_MULTIPLY, 1, m, &cBinop_E0krvJiJ_sendMessage);
}

void Heavy_prog::cMsg_jLTUiXxx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_9D1kG3yV_sendMessage);
}

void Heavy_prog::cBinop_9D1kG3yV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_C5qGIWp4_sendMessage);
}

void Heavy_prog::cMsg_90cSfcQH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_PHekJpKk_sendMessage);
}

void Heavy_prog::cBinop_PHekJpKk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_nOZAkW0f, HV_BINOP_MULTIPLY, 1, m, &cBinop_nOZAkW0f_sendMessage);
}

void Heavy_prog::cBinop_MJ8OwUco_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_IgM69xDY, HV_BINOP_MULTIPLY, 1, m, &cBinop_IgM69xDY_sendMessage);
}

void Heavy_prog::cMsg_FBNu9Dws_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_hLW2Cbm4_sendMessage);
}

void Heavy_prog::cBinop_hLW2Cbm4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_MJ8OwUco_sendMessage);
}

void Heavy_prog::cBinop_GdlAPtXj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_iblGHuAd, 1, m, &cVar_iblGHuAd_sendMessage);
}

void Heavy_prog::cBinop_X0SgaKEZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_G1B7jfBh, HV_BINOP_MULTIPLY, 1, m, &cBinop_G1B7jfBh_sendMessage);
}

void Heavy_prog::cMsg_WeF308Jw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_nWQ9bEPS_sendMessage);
}

void Heavy_prog::cBinop_nWQ9bEPS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_b09mYft7, HV_BINOP_MULTIPLY, 1, m, &cBinop_b09mYft7_sendMessage);
}

void Heavy_prog::cVar_iblGHuAd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_9HBR4aYb_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_p14vECrQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_4bhSD4eo_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 20000.0f, 0, m, &cBinop_PEeSVqgv_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_L8ExTx4z_sendMessage);
}

void Heavy_prog::cVar_2hxr9pSY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 100.0f, 0, m, &cBinop_oV1xRaEd_sendMessage);
}

void Heavy_prog::cCast_qBRVoP3w_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_COS, m, &cUnop_TLxpHWn4_sendMessage);
}

void Heavy_prog::cCast_h2nIFqoO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_SIN, m, &cUnop_aNuuoAVO_sendMessage);
}

void Heavy_prog::cSend_Mw6hBWnQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_hEXxY5BZ_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_CaMPbp8r_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_jLGrvod3_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_oR6ck42z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_UZ3q3013_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_w7zCFVlF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_r8NN2TJX_sendMessage);
}

void Heavy_prog::cBinop_r8NN2TJX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_wXpL7Lha, HV_BINOP_MULTIPLY, 1, m, &cBinop_wXpL7Lha_sendMessage);
}

void Heavy_prog::cBinop_wXpL7Lha_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_fqp7Myrp_sendMessage);
}

void Heavy_prog::cBinop_fqp7Myrp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_oR6ck42z_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_9HBR4aYb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_ivRj16Q7_sendMessage);
}

void Heavy_prog::cBinop_ivRj16Q7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Qo7taQdE_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Elile6Ph_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Ib2rX6GH_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_AUCVLKY2_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_cgFXt2fk_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Wq0BpZ9K_sendMessage);
}

void Heavy_prog::cBinop_E0krvJiJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_xIRbad3Y_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_nOZAkW0f_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_qDw6EBd8_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_IgM69xDY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_ZRBId9kl_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_G1B7jfBh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_BNnlPypU_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_b09mYft7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_h7mZI4oM_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_8hNjd5Zt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_2hxr9pSY, 1, m, &cVar_2hxr9pSY_sendMessage);
}

void Heavy_prog::cCast_dcz7uqqa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_p14vECrQ, 0, m, &cVar_p14vECrQ_sendMessage);
}

void Heavy_prog::cCast_Wq0BpZ9K_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_E0krvJiJ, HV_BINOP_MULTIPLY, 0, m, &cBinop_E0krvJiJ_sendMessage);
}

void Heavy_prog::cCast_Ib2rX6GH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_G1B7jfBh, HV_BINOP_MULTIPLY, 0, m, &cBinop_G1B7jfBh_sendMessage);
}

void Heavy_prog::cCast_cgFXt2fk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_nOZAkW0f, HV_BINOP_MULTIPLY, 0, m, &cBinop_nOZAkW0f_sendMessage);
}

void Heavy_prog::cCast_Elile6Ph_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_b09mYft7, HV_BINOP_MULTIPLY, 0, m, &cBinop_b09mYft7_sendMessage);
}

void Heavy_prog::cCast_AUCVLKY2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_IgM69xDY, HV_BINOP_MULTIPLY, 0, m, &cBinop_IgM69xDY_sendMessage);
}

void Heavy_prog::cCast_Qo7taQdE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_4bhSD4eo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_2hxr9pSY, 0, m, &cVar_2hxr9pSY_sendMessage);
}

void Heavy_prog::cCast_L8ExTx4z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_iblGHuAd, 0, m, &cVar_iblGHuAd_sendMessage);
}

void Heavy_prog::cMsg_CsCVfcqI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_MOufmpiq, 0, m, NULL);
}

void Heavy_prog::cMsg_AJLFeUjk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_AHZMYFM8, 0, m, NULL);
}

void Heavy_prog::cMsg_858i9EJ7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_OSGuqlCO, 0, m, NULL);
}

void Heavy_prog::cMsg_1yRQzBUi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_cSgka7VF, 0, m, NULL);
}

void Heavy_prog::cMsg_VVknRO62_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_zGcTLAb4, 0, m, NULL);
}

void Heavy_prog::cMsg_Waw8jYxf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_JE8gGX3K_sendMessage);
}

void Heavy_prog::cSystem_JE8gGX3K_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ovXrdvNy, HV_BINOP_DIVIDE, 1, m, &cBinop_ovXrdvNy_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_9AslioMi_sendMessage);
}

void Heavy_prog::cUnop_KAPsp4pb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_3PENTZgZ_sendMessage);
}

void Heavy_prog::cMsg_3V5YLsyF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cUnop_onMessage(_c, HV_UNOP_ATAN, m, &cUnop_KAPsp4pb_sendMessage);
}

void Heavy_prog::cBinop_3PENTZgZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ovXrdvNy, HV_BINOP_DIVIDE, 0, m, &cBinop_ovXrdvNy_sendMessage);
}

void Heavy_prog::cCast_9AslioMi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_3V5YLsyF_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_ovXrdvNy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_LT3MVoYV, HV_BINOP_MULTIPLY, 1, m, &cBinop_LT3MVoYV_sendMessage);
}

void Heavy_prog::cBinop_LT3MVoYV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_kFQTqku7_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_UDPU85c8_sendMessage);
}

void Heavy_prog::cUnop_QQ2SoAfN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_vM7aObUm_sendMessage(_c, 0, m);
}

void Heavy_prog::cUnop_jJymRZ7M_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_LmXbudnO_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_BbrkAfAT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 20.0f, 0, m, &cBinop_ptxB26vx_sendMessage);
}

void Heavy_prog::cBinop_ptxB26vx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_LT3MVoYV, HV_BINOP_MULTIPLY, 0, m, &cBinop_LT3MVoYV_sendMessage);
}

void Heavy_prog::cBinop_cxvsBEub_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.1f, 0, m, &cBinop_0Ye9PiP4_sendMessage);
}

void Heavy_prog::cBinop_0Ye9PiP4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_OPcrdZoy_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_CrZngIgT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4MgQBkQ1, HV_BINOP_MULTIPLY, 1, m, &cBinop_4MgQBkQ1_sendMessage);
}

void Heavy_prog::cMsg_rTn8mKNi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_gtkF0WPg_sendMessage);
}

void Heavy_prog::cBinop_gtkF0WPg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_CrZngIgT_sendMessage);
}

void Heavy_prog::cMsg_6nyoY1Ml_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_zJGk3HGB_sendMessage);
}

void Heavy_prog::cBinop_zJGk3HGB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_c2SPWkuP, HV_BINOP_MULTIPLY, 1, m, &cBinop_c2SPWkuP_sendMessage);
}

void Heavy_prog::cBinop_eHL0bDeq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ZDh490Jg, HV_BINOP_MULTIPLY, 1, m, &cBinop_ZDh490Jg_sendMessage);
}

void Heavy_prog::cMsg_l4PPbctb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_usn9Sp1k_sendMessage);
}

void Heavy_prog::cBinop_usn9Sp1k_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_eHL0bDeq_sendMessage);
}

void Heavy_prog::cBinop_ht5S2mWD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_brbkR7aA, 1, m, &cVar_brbkR7aA_sendMessage);
}

void Heavy_prog::cBinop_B31eEX7w_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_BAL6umtX, HV_BINOP_MULTIPLY, 1, m, &cBinop_BAL6umtX_sendMessage);
}

void Heavy_prog::cMsg_bhl6EWEB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_Mj30KeLT_sendMessage);
}

void Heavy_prog::cBinop_Mj30KeLT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_P8OZtvdS, HV_BINOP_MULTIPLY, 1, m, &cBinop_P8OZtvdS_sendMessage);
}

void Heavy_prog::cVar_brbkR7aA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_OfYu4qk1_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_trSuFGlB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_8Pg6m9un_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 20000.0f, 0, m, &cBinop_BbrkAfAT_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_O4BWzubF_sendMessage);
}

void Heavy_prog::cVar_2RilFZLD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 100.0f, 0, m, &cBinop_cxvsBEub_sendMessage);
}

void Heavy_prog::cCast_kFQTqku7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_SIN, m, &cUnop_QQ2SoAfN_sendMessage);
}

void Heavy_prog::cCast_UDPU85c8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_COS, m, &cUnop_jJymRZ7M_sendMessage);
}

void Heavy_prog::cSend_vM7aObUm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_1uYiVQSR_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_LmXbudnO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_ujFLffTo_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_tXTd7U0w_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_RjUHK0UV_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_OPcrdZoy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_hgOuZcLK_sendMessage);
}

void Heavy_prog::cBinop_hgOuZcLK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_e5WuT1Hu, HV_BINOP_MULTIPLY, 1, m, &cBinop_e5WuT1Hu_sendMessage);
}

void Heavy_prog::cBinop_e5WuT1Hu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_IAwrWn6p_sendMessage);
}

void Heavy_prog::cBinop_IAwrWn6p_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_tXTd7U0w_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_OfYu4qk1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_Tl0BZcHJ_sendMessage);
}

void Heavy_prog::cBinop_Tl0BZcHJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_jyEfoZvU_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zgkZpdLr_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ySKdgX3O_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xxvi12T9_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ExnkdNbo_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_eLJuuUnl_sendMessage);
}

void Heavy_prog::cBinop_4MgQBkQ1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_CsCVfcqI_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_c2SPWkuP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_AJLFeUjk_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_ZDh490Jg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_858i9EJ7_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_BAL6umtX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_1yRQzBUi_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_P8OZtvdS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_VVknRO62_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_gFUTD7dh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_2RilFZLD, 1, m, &cVar_2RilFZLD_sendMessage);
}

void Heavy_prog::cCast_WfXsNodN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_trSuFGlB, 0, m, &cVar_trSuFGlB_sendMessage);
}

void Heavy_prog::cCast_jyEfoZvU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_zgkZpdLr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_P8OZtvdS, HV_BINOP_MULTIPLY, 0, m, &cBinop_P8OZtvdS_sendMessage);
}

void Heavy_prog::cCast_eLJuuUnl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4MgQBkQ1, HV_BINOP_MULTIPLY, 0, m, &cBinop_4MgQBkQ1_sendMessage);
}

void Heavy_prog::cCast_ExnkdNbo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_c2SPWkuP, HV_BINOP_MULTIPLY, 0, m, &cBinop_c2SPWkuP_sendMessage);
}

void Heavy_prog::cCast_ySKdgX3O_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_BAL6umtX, HV_BINOP_MULTIPLY, 0, m, &cBinop_BAL6umtX_sendMessage);
}

void Heavy_prog::cCast_xxvi12T9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ZDh490Jg, HV_BINOP_MULTIPLY, 0, m, &cBinop_ZDh490Jg_sendMessage);
}

void Heavy_prog::cCast_8Pg6m9un_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_2RilFZLD, 0, m, &cVar_2RilFZLD_sendMessage);
}

void Heavy_prog::cCast_O4BWzubF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_brbkR7aA, 0, m, &cVar_brbkR7aA_sendMessage);
}

void Heavy_prog::cVar_FDeTeUkm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 10.0f, 0, m, &cBinop_TNZuIqvH_sendMessage);
}

void Heavy_prog::cSlice_8sJM8Vqs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_j5PIOWCl_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_uvVB9JWN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_bqREttei, 0, m, &cIf_bqREttei_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_KCeKetXN, 0, m, &cIf_KCeKetXN_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_aVtOtDdW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_Cb8vZZnJ, 0, m, &cIf_Cb8vZZnJ_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_TDDAYhi7, 0, m, &cIf_TDDAYhi7_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_3JPv7bcq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_Ba3PkdVW_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_1kT39VZZ, 0, m, &cIf_1kT39VZZ_sendMessage);
}

void Heavy_prog::cUnop_Kkrer52v_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_i9Azc6na_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_Hh9DAixe, HV_BINOP_EQ, 1, m, &cBinop_Hh9DAixe_sendMessage);
}

void Heavy_prog::cUnop_DK8wZh0o_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_i9Azc6na_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_Hh9DAixe, HV_BINOP_EQ, 1, m, &cBinop_Hh9DAixe_sendMessage);
}

void Heavy_prog::cIf_1kT39VZZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_DK8wZh0o_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_Kkrer52v_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_Ba3PkdVW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_1kT39VZZ, 1, m, &cIf_1kT39VZZ_sendMessage);
}

void Heavy_prog::cIf_KCeKetXN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_TDDAYhi7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_YpsfX58m_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_bqREttei_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_Cb8vZZnJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_YpsfX58m_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_j5PIOWCl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Hh9DAixe, HV_BINOP_EQ, 0, m, &cBinop_Hh9DAixe_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_M6Z24jVb_sendMessage);
}

void Heavy_prog::cBinop_i9Azc6na_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_qQb3izkV_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_rtfgFjME_sendMessage);
}

void Heavy_prog::cBinop_Hh9DAixe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5M9VwGJ3_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Wq9rwNwY_sendMessage);
}

void Heavy_prog::cCast_Wq9rwNwY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_Cb8vZZnJ, 1, m, &cIf_Cb8vZZnJ_sendMessage);
}

void Heavy_prog::cCast_5M9VwGJ3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_KCeKetXN, 1, m, &cIf_KCeKetXN_sendMessage);
}

void Heavy_prog::cCast_qQb3izkV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_bqREttei, 1, m, &cIf_bqREttei_sendMessage);
}

void Heavy_prog::cCast_rtfgFjME_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_TDDAYhi7, 1, m, &cIf_TDDAYhi7_sendMessage);
}

void Heavy_prog::cBinop_M6Z24jVb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cBinop_7DMtRpTP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_b1MHLKJQ, 0, m, &cVar_b1MHLKJQ_sendMessage);
}

void Heavy_prog::cBinop_YpsfX58m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 12.0f, 0, m, &cBinop_EVxoyFvb_sendMessage);
}

void Heavy_prog::cBinop_EVxoyFvb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_FJWptXfR, HV_BINOP_POW, 1, m, &cBinop_FJWptXfR_sendMessage);
  cMsg_81s74Lfv_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_FJWptXfR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 440.0f, 0, m, &cBinop_7DMtRpTP_sendMessage);
}

void Heavy_prog::cMsg_81s74Lfv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 2.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_FJWptXfR, HV_BINOP_POW, 0, m, &cBinop_FJWptXfR_sendMessage);
}

void Heavy_prog::cVar_pkcTTgHR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_7rrctse1_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_y7UhwfMD, 0, m, &cIf_y7UhwfMD_sendMessage);
}

void Heavy_prog::cUnop_cBLNrtO3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_biFq9DuF_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_3Jf8F2R6_sendMessage);
}

void Heavy_prog::cUnop_dP7rieXb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_biFq9DuF_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_3Jf8F2R6_sendMessage);
}

void Heavy_prog::cIf_y7UhwfMD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_dP7rieXb_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_cBLNrtO3_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_7rrctse1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_y7UhwfMD, 1, m, &cIf_y7UhwfMD_sendMessage);
}

void Heavy_prog::cVar_BdiVtWFA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_nKELIXKr_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_8nDKFkdS, 0, m, &cIf_8nDKFkdS_sendMessage);
}

void Heavy_prog::cUnop_N4xXE4qV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xd5TJTBe_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zhDVXnl6_sendMessage);
}

void Heavy_prog::cUnop_XHithGxE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xd5TJTBe_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zhDVXnl6_sendMessage);
}

void Heavy_prog::cIf_8nDKFkdS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_XHithGxE_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_N4xXE4qV_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_nKELIXKr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_8nDKFkdS, 1, m, &cIf_8nDKFkdS_sendMessage);
}

void Heavy_prog::cSlice_cY26xX6b_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_1ogW5nXH_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_beD3LScr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_RqxnlCKS, 0, m, &cIf_RqxnlCKS_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_nHoBhO2B, HV_BINOP_EQ, 0, m, &cBinop_nHoBhO2B_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_0BzyKePF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_cHUWc560, 0, m, &cIf_cHUWc560_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_omnTRxhK, 0, m, &cIf_omnTRxhK_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_cHUWc560_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 3.0f, 0, m, &cBinop_gT0696cZ_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_RqxnlCKS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_omnTRxhK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_KzIKDkqD, 0, m, &cIf_KzIKDkqD_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_7jAdJNx3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_c5lwRKa2, 0, m, &cIf_c5lwRKa2_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_KzIKDkqD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_dlklO1b8, 0, m, &cIf_dlklO1b8_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_dlklO1b8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 3.0f, 0, m, &cBinop_gT0696cZ_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_c5lwRKa2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_SUXaUi8J_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_B8arjK07, HV_BINOP_EQ, 0, m, &cBinop_B8arjK07_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_yzWfyBqm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_HpXknl5P_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_7jAdJNx3, 0, m, &cIf_7jAdJNx3_sendMessage);
}

void Heavy_prog::cBinop_1ogW5nXH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_xx15Wh7J_sendMessage);
}

void Heavy_prog::cBinop_xx15Wh7J_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_osvpt5Hx_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_KWQcQaC7_sendMessage);
}

void Heavy_prog::cBinop_nHoBhO2B_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_869OGQCT_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_P1yl6cbw_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_YokBG01J_sendMessage);
}

void Heavy_prog::cCast_biFq9DuF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_nHoBhO2B, HV_BINOP_EQ, 1, m, &cBinop_nHoBhO2B_sendMessage);
}

void Heavy_prog::cCast_3Jf8F2R6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_TWFFBgLs_sendMessage);
}

void Heavy_prog::cBinop_TWFFBgLs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jL95UeaB_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_W7DN3Lny_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_1TnsdiJO_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_aseozbt4_sendMessage);
}

void Heavy_prog::cBinop_W5dK6CDt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_KzIKDkqD, 1, m, &cIf_KzIKDkqD_sendMessage);
}

void Heavy_prog::cCast_zhDVXnl6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_B8arjK07, HV_BINOP_EQ, 1, m, &cBinop_B8arjK07_sendMessage);
}

void Heavy_prog::cCast_xd5TJTBe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_PXUAYFXu_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_NctS3pOZ_sendMessage);
}

void Heavy_prog::cBinop_PXUAYFXu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_SUXaUi8J, 1, m, &cIf_SUXaUi8J_sendMessage);
}

void Heavy_prog::cCast_KWQcQaC7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_iRSFNZFw_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_TfcPMZPj_sendMessage);
}

void Heavy_prog::cCast_osvpt5Hx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_SUXaUi8J, 0, m, &cIf_SUXaUi8J_sendMessage);
}

void Heavy_prog::cBinop_NctS3pOZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_c5lwRKa2, 1, m, &cIf_c5lwRKa2_sendMessage);
}

void Heavy_prog::cBinop_B8arjK07_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_dlklO1b8, 1, m, &cIf_dlklO1b8_sendMessage);
}

void Heavy_prog::cCast_TfcPMZPj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_HpXknl5P, 1, m, &cVar_HpXknl5P_sendMessage);
}

void Heavy_prog::cCast_iRSFNZFw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_yzWfyBqm, 0, m, &cIf_yzWfyBqm_sendMessage);
}

void Heavy_prog::cCast_869OGQCT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_7jAdJNx3, 1, m, &cIf_7jAdJNx3_sendMessage);
}

void Heavy_prog::cCast_YokBG01J_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_omnTRxhK, 1, m, &cIf_omnTRxhK_sendMessage);
}

void Heavy_prog::cCast_P1yl6cbw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_HpXknl5P, 0, m, &cVar_HpXknl5P_sendMessage);
}

void Heavy_prog::cCast_aseozbt4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_W5dK6CDt_sendMessage);
}

void Heavy_prog::cCast_W7DN3Lny_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_RqxnlCKS, 1, m, &cIf_RqxnlCKS_sendMessage);
}

void Heavy_prog::cCast_1TnsdiJO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_cHUWc560, 1, m, &cIf_cHUWc560_sendMessage);
}

void Heavy_prog::cCast_jL95UeaB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_yzWfyBqm, 1, m, &cIf_yzWfyBqm_sendMessage);
}

void Heavy_prog::cVar_wuRMR9le_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_mENM0aIY_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_DALmR1vY, 0, m, &cIf_DALmR1vY_sendMessage);
}

void Heavy_prog::cUnop_kSQLd652_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_A4ZWoyGJ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ozSr3fge_sendMessage);
}

void Heavy_prog::cUnop_2o2J7yiH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_A4ZWoyGJ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ozSr3fge_sendMessage);
}

void Heavy_prog::cIf_DALmR1vY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_2o2J7yiH_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_kSQLd652_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_mENM0aIY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_DALmR1vY, 1, m, &cIf_DALmR1vY_sendMessage);
}

void Heavy_prog::cVar_vXZZN7Wo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_jy7ruBoM_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_7p8993MK, 0, m, &cIf_7p8993MK_sendMessage);
}

void Heavy_prog::cUnop_YkchkR4r_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_CvawPewA_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_V3PpEJjW_sendMessage);
}

void Heavy_prog::cUnop_cPDt1za1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_CvawPewA_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_V3PpEJjW_sendMessage);
}

void Heavy_prog::cIf_7p8993MK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_cPDt1za1_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_YkchkR4r_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_jy7ruBoM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_7p8993MK, 1, m, &cIf_7p8993MK_sendMessage);
}

void Heavy_prog::cSlice_9cShn2pW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_Ty8qMA64_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_nww5UdEW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_J9cTGCzO, 0, m, &cIf_J9cTGCzO_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_Phgf8WwC, HV_BINOP_EQ, 0, m, &cBinop_Phgf8WwC_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_r5W61ffA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_MXJfx3O8, 0, m, &cIf_MXJfx3O8_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_bwU2VgGw, 0, m, &cIf_bwU2VgGw_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_MXJfx3O8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_bBkXyPVt, 0, m, &cVar_bBkXyPVt_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_J9cTGCzO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_bwU2VgGw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_K1D1JO56, 0, m, &cIf_K1D1JO56_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_uh8beHpa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_E5pIRyr8, 0, m, &cIf_E5pIRyr8_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_K1D1JO56_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_zaiWJWpD, 0, m, &cIf_zaiWJWpD_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_zaiWJWpD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_bBkXyPVt, 0, m, &cVar_bBkXyPVt_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_E5pIRyr8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_tKJwzIqi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_RGapAWdH, HV_BINOP_EQ, 0, m, &cBinop_RGapAWdH_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_USaaUumh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_l6GM6Mzf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_uh8beHpa, 0, m, &cIf_uh8beHpa_sendMessage);
}

void Heavy_prog::cBinop_Ty8qMA64_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_4BiuNKLc_sendMessage);
}

void Heavy_prog::cBinop_4BiuNKLc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jcYkVrjK_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5m5dPE7d_sendMessage);
}

void Heavy_prog::cBinop_Phgf8WwC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hVULbzNk_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_XxtpfUNu_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_3u5tUqvt_sendMessage);
}

void Heavy_prog::cCast_A4ZWoyGJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Phgf8WwC, HV_BINOP_EQ, 1, m, &cBinop_Phgf8WwC_sendMessage);
}

void Heavy_prog::cCast_ozSr3fge_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_KCtfqJxW_sendMessage);
}

void Heavy_prog::cBinop_KCtfqJxW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hpz2L3gN_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_v0Vi1okB_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_u2cFFkuN_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_vCCyswCE_sendMessage);
}

void Heavy_prog::cBinop_Cfb1SX5N_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_K1D1JO56, 1, m, &cIf_K1D1JO56_sendMessage);
}

void Heavy_prog::cCast_V3PpEJjW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_RGapAWdH, HV_BINOP_EQ, 1, m, &cBinop_RGapAWdH_sendMessage);
}

void Heavy_prog::cCast_CvawPewA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_RwIM4p8U_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_TqJjFuyo_sendMessage);
}

void Heavy_prog::cBinop_RwIM4p8U_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_tKJwzIqi, 1, m, &cIf_tKJwzIqi_sendMessage);
}

void Heavy_prog::cCast_5m5dPE7d_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zuDPzebT_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_9ODalDEU_sendMessage);
}

void Heavy_prog::cCast_jcYkVrjK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_tKJwzIqi, 0, m, &cIf_tKJwzIqi_sendMessage);
}

void Heavy_prog::cBinop_TqJjFuyo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_E5pIRyr8, 1, m, &cIf_E5pIRyr8_sendMessage);
}

void Heavy_prog::cBinop_RGapAWdH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_zaiWJWpD, 1, m, &cIf_zaiWJWpD_sendMessage);
}

void Heavy_prog::cCast_zuDPzebT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_USaaUumh, 0, m, &cIf_USaaUumh_sendMessage);
}

void Heavy_prog::cCast_9ODalDEU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_l6GM6Mzf, 1, m, &cVar_l6GM6Mzf_sendMessage);
}

void Heavy_prog::cCast_hVULbzNk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_uh8beHpa, 1, m, &cIf_uh8beHpa_sendMessage);
}

void Heavy_prog::cCast_XxtpfUNu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_l6GM6Mzf, 0, m, &cVar_l6GM6Mzf_sendMessage);
}

void Heavy_prog::cCast_3u5tUqvt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_bwU2VgGw, 1, m, &cIf_bwU2VgGw_sendMessage);
}

void Heavy_prog::cCast_u2cFFkuN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_MXJfx3O8, 1, m, &cIf_MXJfx3O8_sendMessage);
}

void Heavy_prog::cCast_hpz2L3gN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_USaaUumh, 1, m, &cIf_USaaUumh_sendMessage);
}

void Heavy_prog::cCast_v0Vi1okB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_J9cTGCzO, 1, m, &cIf_J9cTGCzO_sendMessage);
}

void Heavy_prog::cCast_vCCyswCE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_Cfb1SX5N_sendMessage);
}

void Heavy_prog::cVar_Hg9kPjvt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_jCPwJYU6_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_bknadFO3, 0, m, &cIf_bknadFO3_sendMessage);
}

void Heavy_prog::cUnop_gQTK6as0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_suD4g640_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_nMFg89Hc_sendMessage);
}

void Heavy_prog::cUnop_bAVDGKT0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_suD4g640_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_nMFg89Hc_sendMessage);
}

void Heavy_prog::cIf_bknadFO3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_bAVDGKT0_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_gQTK6as0_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_jCPwJYU6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_bknadFO3, 1, m, &cIf_bknadFO3_sendMessage);
}

void Heavy_prog::cVar_yeV5qANo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_APGqyKRi_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_oJeYhaVK, 0, m, &cIf_oJeYhaVK_sendMessage);
}

void Heavy_prog::cUnop_GxFDyPpo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_BkmNLKEQ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_EuFhTvuA_sendMessage);
}

void Heavy_prog::cUnop_lvxe5nfm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_BkmNLKEQ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_EuFhTvuA_sendMessage);
}

void Heavy_prog::cIf_oJeYhaVK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_lvxe5nfm_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_GxFDyPpo_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_APGqyKRi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_oJeYhaVK, 1, m, &cIf_oJeYhaVK_sendMessage);
}

void Heavy_prog::cSlice_9b1oKPAz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_9OkjzObC_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_BhbzN3lY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_9TwSE3l9, 0, m, &cIf_9TwSE3l9_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_8eTeWi4z, HV_BINOP_EQ, 0, m, &cBinop_8eTeWi4z_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_LoH4l7Du_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_4fg45QhZ, 0, m, &cIf_4fg45QhZ_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_EC9nV2d6, 0, m, &cIf_EC9nV2d6_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_4fg45QhZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_FDeTeUkm, 0, m, &cVar_FDeTeUkm_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_9TwSE3l9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_EC9nV2d6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_CTHQlZd3, 0, m, &cIf_CTHQlZd3_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_K9wcvEmq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_HCQkqk7n, 0, m, &cIf_HCQkqk7n_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_CTHQlZd3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_uyJsdmNE, 0, m, &cIf_uyJsdmNE_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_uyJsdmNE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_FDeTeUkm, 0, m, &cVar_FDeTeUkm_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_HCQkqk7n_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_5P7qD57V_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_6415d2ls, HV_BINOP_EQ, 0, m, &cBinop_6415d2ls_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_BAEBVWF3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_e7jY34PG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_K9wcvEmq, 0, m, &cIf_K9wcvEmq_sendMessage);
}

void Heavy_prog::cBinop_9OkjzObC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_0kQOEwc5_sendMessage);
}

void Heavy_prog::cBinop_0kQOEwc5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_w7Zs8ztG_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_pEz8MQBB_sendMessage);
}

void Heavy_prog::cBinop_8eTeWi4z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_CwMtuekB_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_T0rZKqOj_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_tVsl67Ys_sendMessage);
}

void Heavy_prog::cCast_nMFg89Hc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_zPsBujTE_sendMessage);
}

void Heavy_prog::cCast_suD4g640_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_8eTeWi4z, HV_BINOP_EQ, 1, m, &cBinop_8eTeWi4z_sendMessage);
}

void Heavy_prog::cBinop_zPsBujTE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_iyMAp8zg_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_mgTVhVns_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_M6InNXli_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_0mTgdED7_sendMessage);
}

void Heavy_prog::cBinop_PtcYLTeE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_CTHQlZd3, 1, m, &cIf_CTHQlZd3_sendMessage);
}

void Heavy_prog::cCast_BkmNLKEQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_UctSz5m6_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_4QGCuq98_sendMessage);
}

void Heavy_prog::cCast_EuFhTvuA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_6415d2ls, HV_BINOP_EQ, 1, m, &cBinop_6415d2ls_sendMessage);
}

void Heavy_prog::cBinop_UctSz5m6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_5P7qD57V, 1, m, &cIf_5P7qD57V_sendMessage);
}

void Heavy_prog::cCast_w7Zs8ztG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_5P7qD57V, 0, m, &cIf_5P7qD57V_sendMessage);
}

void Heavy_prog::cCast_pEz8MQBB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_YyqM5KPp_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Z2sfAU6f_sendMessage);
}

void Heavy_prog::cBinop_4QGCuq98_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_HCQkqk7n, 1, m, &cIf_HCQkqk7n_sendMessage);
}

void Heavy_prog::cBinop_6415d2ls_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_uyJsdmNE, 1, m, &cIf_uyJsdmNE_sendMessage);
}

void Heavy_prog::cCast_Z2sfAU6f_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_e7jY34PG, 1, m, &cVar_e7jY34PG_sendMessage);
}

void Heavy_prog::cCast_YyqM5KPp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_BAEBVWF3, 0, m, &cIf_BAEBVWF3_sendMessage);
}

void Heavy_prog::cCast_T0rZKqOj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_e7jY34PG, 0, m, &cVar_e7jY34PG_sendMessage);
}

void Heavy_prog::cCast_CwMtuekB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_K9wcvEmq, 1, m, &cIf_K9wcvEmq_sendMessage);
}

void Heavy_prog::cCast_tVsl67Ys_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_EC9nV2d6, 1, m, &cIf_EC9nV2d6_sendMessage);
}

void Heavy_prog::cCast_0mTgdED7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_PtcYLTeE_sendMessage);
}

void Heavy_prog::cCast_M6InNXli_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_4fg45QhZ, 1, m, &cIf_4fg45QhZ_sendMessage);
}

void Heavy_prog::cCast_iyMAp8zg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_BAEBVWF3, 1, m, &cIf_BAEBVWF3_sendMessage);
}

void Heavy_prog::cCast_mgTVhVns_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_9TwSE3l9, 1, m, &cIf_9TwSE3l9_sendMessage);
}

void Heavy_prog::cSlice_IxJCHTdk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_wA96TufP_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_1VB5q5bI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Gsmd2NEw_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_GSMTPsS9_sendMessage);
      cSwitchcase_MjKXfbnC_onMessage(_c, NULL, 0, m, NULL);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_wA96TufP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cSystem_HwP073OS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_XMU3d1kd, HV_BINOP_SUBTRACT, 1, m, &cBinop_XMU3d1kd_sendMessage);
}

void Heavy_prog::cMsg_xIKKG47Q_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "currentTime");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_HwP073OS_sendMessage);
}

void Heavy_prog::cBinop_XMU3d1kd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_rvjA5qjA, HV_BINOP_DIVIDE, 0, m, &cBinop_rvjA5qjA_sendMessage);
}

void Heavy_prog::cSystem_eaWm2ZJO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_XMU3d1kd, HV_BINOP_SUBTRACT, 0, m, &cBinop_XMU3d1kd_sendMessage);
}

void Heavy_prog::cMsg_uuBMD5Sp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "currentTime");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_eaWm2ZJO_sendMessage);
}

void Heavy_prog::cBinop_rvjA5qjA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_uRP4F2or_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cSystem_LevIZfYA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_I9GlOl73_sendMessage);
}

void Heavy_prog::cMsg_WC8iWjSx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_LevIZfYA_sendMessage);
}

void Heavy_prog::cBinop_I9GlOl73_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_rvjA5qjA, HV_BINOP_DIVIDE, 1, m, &cBinop_rvjA5qjA_sendMessage);
}

void Heavy_prog::cSwitchcase_uRP4F2or_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_TssKvVbZ_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_TssKvVbZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_t68k8hSW_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_vFRTsYxE_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Zd67motC_sendMessage);
}

void Heavy_prog::cIf_oLoIHh0h_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_xuFLU1OF, 0, m, &cVar_xuFLU1OF_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_bNhD7Qh5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_oLoIHh0h, 1, m, &cIf_oLoIHh0h_sendMessage);
}

void Heavy_prog::cVar_xuFLU1OF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_4WZx7vW9_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN_EQL, 96.0f, 0, m, &cBinop_jtqREQPt_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_1zSV3iaP, 0, m, &cIf_1zSV3iaP_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MOD_UNIPOLAR, 4.0f, 0, m, &cBinop_5g7TIFBq_sendMessage);
}

void Heavy_prog::cVar_s2qz4Wip_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_BnTnmTdX_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_upW7yv77_sendMessage);
}

void Heavy_prog::cSwitchcase_MjKXfbnC_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x437A0000: { // "250.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_MOLUAbtQ_sendMessage);
      break;
    }
    case 0x437C0000: { // "252.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_u5KHTJOH_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_MOLUAbtQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_3yxVrNxB_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_u5KHTJOH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_DOhyLgDW_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cSwitchcase_DOhyLgDW_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cMsg_hClYh5gg_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cMsg_hClYh5gg_sendMessage(_c, 0, m);
      cDelay_onMessage(_c, &Context(_c)->cDelay_uMMHGtXd, 1, m, &cDelay_uMMHGtXd_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_heDHvWmT_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_uMMHGtXd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_uMMHGtXd, m);
  cMsg_eY75DkqR_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_hClYh5gg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_uMMHGtXd, 0, m, &cDelay_uMMHGtXd_sendMessage);
}

void Heavy_prog::cCast_heDHvWmT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_uMMHGtXd, 0, m, &cDelay_uMMHGtXd_sendMessage);
}

void Heavy_prog::cIf_1zSV3iaP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_21YrAT3G_sendMessage);
      break;
    }
    case 1: {
      cMsg_snu76p5G_sendMessage(_c, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_jtqREQPt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_1zSV3iaP, 1, m, &cIf_1zSV3iaP_sendMessage);
}

void Heavy_prog::cVar_qN5mjKXS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_xuFLU1OF, 1, m, &cVar_xuFLU1OF_sendMessage);
}

void Heavy_prog::cSwitchcase_D0fIE7k6_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_CxIv38qT_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_CxIv38qT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_YXU6ke3k, 0, m, &cVar_YXU6ke3k_sendMessage);
}

void Heavy_prog::cVar_FRH45GIQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_RdBW6jhU_sendMessage);
  cSwitchcase_79zqwKql_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cVar_YXU6ke3k_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_WvvUtsZV_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MOD_UNIPOLAR, 16.0f, 0, m, &cBinop_A7Mi0D5O_sendMessage);
}

void Heavy_prog::cSwitchcase_9acPPosp_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cMsg_9Dm2wkyx_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cMsg_9Dm2wkyx_sendMessage(_c, 0, m);
      cDelay_onMessage(_c, &Context(_c)->cDelay_RaYcPW50, 1, m, &cDelay_RaYcPW50_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_dTA7XYyn_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_RaYcPW50_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_RaYcPW50, m);
  cVar_onMessage(_c, &Context(_c)->cVar_bNhD7Qh5, 0, m, &cVar_bNhD7Qh5_sendMessage);
}

void Heavy_prog::cMsg_9Dm2wkyx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_RaYcPW50, 0, m, &cDelay_RaYcPW50_sendMessage);
}

void Heavy_prog::cCast_dTA7XYyn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_RaYcPW50, 0, m, &cDelay_RaYcPW50_sendMessage);
}

void Heavy_prog::cCast_Gsmd2NEw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_WC8iWjSx_sendMessage(_c, 0, m);
  cMsg_uuBMD5Sp_sendMessage(_c, 0, m);
  cIf_onMessage(_c, &Context(_c)->cIf_oLoIHh0h, 0, m, &cIf_oLoIHh0h_sendMessage);
}

void Heavy_prog::cCast_GSMTPsS9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_xIKKG47Q_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_snu76p5G_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_xuFLU1OF, 1, m, &cVar_xuFLU1OF_sendMessage);
}

void Heavy_prog::cBinop_21YrAT3G_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_xuFLU1OF, 1, m, &cVar_xuFLU1OF_sendMessage);
}

void Heavy_prog::cCast_vFRTsYxE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_9acPPosp_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cCast_Zd67motC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_qN5mjKXS, 0, m, &cVar_qN5mjKXS_sendMessage);
}

void Heavy_prog::cCast_t68k8hSW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_s2qz4Wip, 0, m, &cVar_s2qz4Wip_sendMessage);
}

void Heavy_prog::cCast_upW7yv77_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_xuFLU1OF, 0, m, &cVar_xuFLU1OF_sendMessage);
}

void Heavy_prog::cCast_BnTnmTdX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_oLoIHh0h, 1, m, &cIf_oLoIHh0h_sendMessage);
}

void Heavy_prog::cBinop_YFNmdQys_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_RaYcPW50, 1, m, &cDelay_RaYcPW50_sendMessage);
}

void Heavy_prog::cMsg_3yxVrNxB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cPrint_onMessage(_c, m, "start");
  cSwitchcase_uRP4F2or_onMessage(_c, NULL, 0, m, NULL);
  cVar_onMessage(_c, &Context(_c)->cVar_FRH45GIQ, 0, m, &cVar_FRH45GIQ_sendMessage);
}

void Heavy_prog::cMsg_eY75DkqR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cPrint_onMessage(_c, m, "stop");
  cIf_onMessage(_c, &Context(_c)->cIf_oLoIHh0h, 1, m, &cIf_oLoIHh0h_sendMessage);
}

void Heavy_prog::cMsg_EaFjPmCQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 60000.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_YFNmdQys_sendMessage);
}

void Heavy_prog::cBinop_5g7TIFBq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_5bABkYUl_sendMessage);
}

void Heavy_prog::cBinop_5bABkYUl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_D0fIE7k6_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cBinop_WvvUtsZV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_YXU6ke3k, 1, m, &cVar_YXU6ke3k_sendMessage);
}

void Heavy_prog::cBinop_A7Mi0D5O_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_FRH45GIQ, 0, m, &cVar_FRH45GIQ_sendMessage);
}

void Heavy_prog::cVar_SArVFfWE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_EaFjPmCQ_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_4WZx7vW9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_RdBW6jhU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cMsg_QXRjl9VS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_DIB9QSo9_sendMessage);
}

void Heavy_prog::cSystem_DIB9QSo9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4GOoOJQ2, HV_BINOP_MULTIPLY, 1, m, &cBinop_4GOoOJQ2_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_4r6fcEBT, HV_BINOP_MULTIPLY, 1, m, &cBinop_4r6fcEBT_sendMessage);
}

void Heavy_prog::cSwitchcase_oIjNMhKb_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cMsg_mcEsLHdU_sendMessage(_c, 0, m);
      break;
    }
    case 0x7A5B032D: { // "stop"
      cMsg_mcEsLHdU_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_PTD1AvLN_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_zru4532F_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_zru4532F, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_zru4532F, 0, m, &cDelay_zru4532F_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_wGcPHTZz, 0, m, &cVar_wGcPHTZz_sendMessage);
}

void Heavy_prog::cCast_PTD1AvLN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_mcEsLHdU_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_zru4532F, 0, m, &cDelay_zru4532F_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_wGcPHTZz, 0, m, &cVar_wGcPHTZz_sendMessage);
}

void Heavy_prog::cMsg_xn8vU1TN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_axjcUamu_sendMessage);
}

void Heavy_prog::cSystem_axjcUamu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_gDlkn2To_sendMessage);
}

void Heavy_prog::cVar_nu048CLv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_TUWUJLAa, HV_BINOP_MULTIPLY, 0, m, &cBinop_TUWUJLAa_sendMessage);
}

void Heavy_prog::cMsg_mcEsLHdU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_zru4532F, 0, m, &cDelay_zru4532F_sendMessage);
}

void Heavy_prog::cBinop_UyJYhwtm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_zru4532F, 2, m, &cDelay_zru4532F_sendMessage);
}

void Heavy_prog::cBinop_gDlkn2To_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_TUWUJLAa, HV_BINOP_MULTIPLY, 1, m, &cBinop_TUWUJLAa_sendMessage);
}

void Heavy_prog::cBinop_TUWUJLAa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_UyJYhwtm_sendMessage);
}

void Heavy_prog::cVar_wGcPHTZz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_XgulPMUY, HV_BINOP_SUBTRACT, 0, m, &cBinop_XgulPMUY_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_LESS_THAN_EQL, 0.0f, 0, m, &cBinop_W6hajF4Y_sendMessage);
}

void Heavy_prog::cSwitchcase_fW7ItdXN_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_CHQQlMmr_sendMessage);
      break;
    }
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_ZzrUuNxu_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_CHQQlMmr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_WkPWrzFw, 0, m, &cVar_WkPWrzFw_sendMessage);
}

void Heavy_prog::cCast_ZzrUuNxu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_aB5Q4A5M_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_iojY2Biv_sendMessage);
}

void Heavy_prog::cSwitchcase_F1XZivsO_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cSlice_onMessage(_c, &Context(_c)->cSlice_U1jLjdCp, 0, m, &cSlice_U1jLjdCp_sendMessage);
      break;
    }
    case 0x3E004DAB: { // "set"
      cSlice_onMessage(_c, &Context(_c)->cSlice_sUlUYeNs, 0, m, &cSlice_sUlUYeNs_sendMessage);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_8FkHnmkR_sendMessage);
      cSlice_onMessage(_c, &Context(_c)->cSlice_G0EFu0UW, 0, m, &cSlice_G0EFu0UW_sendMessage);
      cSlice_onMessage(_c, &Context(_c)->cSlice_6Za39v3y, 0, m, &cSlice_6Za39v3y_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_VmYW5JXN_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_8HsNmF6j_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cSlice_U1jLjdCp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cMsg_uEDZ8J8m_sendMessage(_c, 0, m);
      break;
    }
    case 1: {
      cMsg_uEDZ8J8m_sendMessage(_c, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_sUlUYeNs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_MUUMX6Lp_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ZU0xyxRF_sendMessage);
      break;
    }
    case 1: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_MUUMX6Lp_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ZU0xyxRF_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_SJWgBMEp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_rbyHYMKl_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_UDKNzv0B_sendMessage);
}

void Heavy_prog::cVar_gHq9nltM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_mKpuIXHt_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cSwitchcase_mKpuIXHt_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_DVcXajr0_sendMessage);
      break;
    }
    default: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_4GOoOJQ2, HV_BINOP_MULTIPLY, 0, m, &cBinop_4GOoOJQ2_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_lTf1xhFW, HV_BINOP_DIVIDE, 1, m, &cBinop_lTf1xhFW_sendMessage);
      cVar_onMessage(_c, &Context(_c)->cVar_nu048CLv, 0, m, &cVar_nu048CLv_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cCast_DVcXajr0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_Erth6emw_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_E23hfvrT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_938vmUmW, HV_BINOP_SUBTRACT, 1, m, &cBinop_938vmUmW_sendMessage);
}

void Heavy_prog::cVar_rod0aeKq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_WkPWrzFw, 0, m, &cVar_WkPWrzFw_sendMessage);
}

void Heavy_prog::cVar_WkPWrzFw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Xnc8lmg9, HV_BINOP_ADD, 0, m, &cBinop_Xnc8lmg9_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_jPneGUvo, HV_BINOP_ADD, 0, m, &cBinop_jPneGUvo_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_XUaSTwuF, m);
  sVarf_onMessage(_c, &Context(_c)->sVarf_SiNH5FLu, m);
}

void Heavy_prog::cSlice_G0EFu0UW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_rbyHYMKl_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_UDKNzv0B_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_6Za39v3y_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_CRXWEIPg_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jWTIr8NF_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_gz7DE89L_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_wGcPHTZz, 1, m, &cVar_wGcPHTZz_sendMessage);
}

void Heavy_prog::cBinop_4r6fcEBT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_gz7DE89L_sendMessage);
}

void Heavy_prog::cBinop_4GOoOJQ2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_Sktf5ZXo_sendMessage);
}

void Heavy_prog::cBinop_Sktf5ZXo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_XgulPMUY, HV_BINOP_SUBTRACT, 1, m, &cBinop_XgulPMUY_sendMessage);
}

void Heavy_prog::cBinop_XgulPMUY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_wGcPHTZz, 1, m, &cVar_wGcPHTZz_sendMessage);
}

void Heavy_prog::cMsg_mW1hMqlR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cSwitchcase_oIjNMhKb_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_SJ6Re7Al_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cSwitchcase_oIjNMhKb_onMessage(_c, NULL, 0, m, NULL);
  cBinop_onMessage(_c, &Context(_c)->cBinop_jPneGUvo, HV_BINOP_ADD, 1, m, &cBinop_jPneGUvo_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_Xnc8lmg9, HV_BINOP_ADD, 1, m, &cBinop_Xnc8lmg9_sendMessage);
}

void Heavy_prog::cBinop_W6hajF4Y_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_fW7ItdXN_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cBinop_Xnc8lmg9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_WkPWrzFw, 1, m, &cVar_WkPWrzFw_sendMessage);
}

void Heavy_prog::cBinop_lTf1xhFW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ygcOjkrG, HV_BINOP_DIVIDE, 1, m, &cBinop_ygcOjkrG_sendMessage);
}

void Heavy_prog::cBinop_ygcOjkrG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_jPneGUvo, HV_BINOP_ADD, 1, m, &cBinop_jPneGUvo_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_Xnc8lmg9, HV_BINOP_ADD, 1, m, &cBinop_Xnc8lmg9_sendMessage);
}

void Heavy_prog::cCast_rbyHYMKl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4r6fcEBT, HV_BINOP_MULTIPLY, 0, m, &cBinop_4r6fcEBT_sendMessage);
}

void Heavy_prog::cCast_UDKNzv0B_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_lTf1xhFW, HV_BINOP_DIVIDE, 0, m, &cBinop_lTf1xhFW_sendMessage);
}

void Heavy_prog::cCast_jWTIr8NF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_938vmUmW, HV_BINOP_SUBTRACT, 0, m, &cBinop_938vmUmW_sendMessage);
}

void Heavy_prog::cCast_CRXWEIPg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_rod0aeKq, 1, m, &cVar_rod0aeKq_sendMessage);
}

void Heavy_prog::cCast_aB5Q4A5M_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_SJ6Re7Al_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_iojY2Biv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_rod0aeKq, 0, m, &cVar_rod0aeKq_sendMessage);
}

void Heavy_prog::cBinop_jPneGUvo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_E23hfvrT, 0, m, &cVar_E23hfvrT_sendMessage);
}

void Heavy_prog::cMsg_uEDZ8J8m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cSwitchcase_oIjNMhKb_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_kipIUGwv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_SJWgBMEp, 1, m, &cVar_SJWgBMEp_sendMessage);
}

void Heavy_prog::cMsg_Erth6emw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 20.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_4GOoOJQ2, HV_BINOP_MULTIPLY, 0, m, &cBinop_4GOoOJQ2_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_lTf1xhFW, HV_BINOP_DIVIDE, 1, m, &cBinop_lTf1xhFW_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_nu048CLv, 0, m, &cVar_nu048CLv_sendMessage);
}

void Heavy_prog::cCast_MUUMX6Lp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_uEDZ8J8m_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_ZU0xyxRF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_iKjL1IIz_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_jPneGUvo, HV_BINOP_ADD, 0, m, &cBinop_jPneGUvo_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_WkPWrzFw, 1, m, &cVar_WkPWrzFw_sendMessage);
}

void Heavy_prog::cBinop_938vmUmW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ygcOjkrG, HV_BINOP_DIVIDE, 0, m, &cBinop_ygcOjkrG_sendMessage);
}

void Heavy_prog::cCast_iKjL1IIz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_SJ6Re7Al_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_8HsNmF6j_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_kipIUGwv_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_VmYW5JXN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_mW1hMqlR_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_8FkHnmkR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_SJWgBMEp, 0, m, &cVar_SJWgBMEp_sendMessage);
}

void Heavy_prog::cSwitchcase_79zqwKql_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x41000000: { // "8.0"
      cSlice_onMessage(_c, &Context(_c)->cSlice_hcqGbkXj, 0, m, &cSlice_hcqGbkXj_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cSlice_hcqGbkXj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cMsg_hAvWo0jh_sendMessage(_c, 0, m);
      break;
    }
    case 1: {
      cMsg_hAvWo0jh_sendMessage(_c, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_gT0696cZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cBinop_0Btjt5FJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 4.0f, 0, m, &cBinop_PyUEWC5o_sendMessage);
}

void Heavy_prog::cCast_a8xmLPlv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_luqHfF5G, HV_BINOP_SUBTRACT, 1, m, &cBinop_luqHfF5G_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_W4dtgSD9, HV_BINOP_ADD, 1, m, &cBinop_W4dtgSD9_sendMessage);
}

void Heavy_prog::cCast_8pCM7VLl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_luqHfF5G, HV_BINOP_SUBTRACT, 0, m, &cBinop_luqHfF5G_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_W4dtgSD9, HV_BINOP_ADD, 0, m, &cBinop_W4dtgSD9_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_b1MHLKJQ, 0, m, &cVar_b1MHLKJQ_sendMessage);
}

void Heavy_prog::cMsg_8DNPKs6T_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 80.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_b1MHLKJQ, 0, m, &cVar_b1MHLKJQ_sendMessage);
}

void Heavy_prog::cBinop_TNZuIqvH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_p14vECrQ, 0, m, &cVar_p14vECrQ_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_trSuFGlB, 0, m, &cVar_trSuFGlB_sendMessage);
}

void Heavy_prog::cBinop_luqHfF5G_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_AFhXb2iL, m);
}

void Heavy_prog::cBinop_W4dtgSD9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_55uGsKPW, m);
}

void Heavy_prog::cBinop_PyUEWC5o_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_a8xmLPlv_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_8pCM7VLl_sendMessage);
}

void Heavy_prog::cCast_v61dL0a8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_luqHfF5G, HV_BINOP_SUBTRACT, 0, m, &cBinop_luqHfF5G_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_W4dtgSD9, HV_BINOP_ADD, 0, m, &cBinop_W4dtgSD9_sendMessage);
}

void Heavy_prog::cCast_xr8p7Jz8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_W4dtgSD9, HV_BINOP_ADD, 0, m, &cBinop_W4dtgSD9_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_luqHfF5G, HV_BINOP_SUBTRACT, 0, m, &cBinop_luqHfF5G_sendMessage);
}

void Heavy_prog::cMsg_oR989Oml_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 120.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_SArVFfWE, 0, m, &cVar_SArVFfWE_sendMessage);
}

void Heavy_prog::cMsg_hAvWo0jh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cSwitchcase_F1XZivsO_onMessage(_c, NULL, 0, m, NULL);
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 200.0f);
  cSwitchcase_F1XZivsO_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cReceive_ijIwaagV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_O9NfMaGc_sendMessage(_c, 0, m);
  cMsg_Waw8jYxf_sendMessage(_c, 0, m);
  cMsg_3CG8bgKq_sendMessage(_c, 0, m);
  cMsg_B3whapdc_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_p14vECrQ, 0, m, &cVar_p14vECrQ_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_trSuFGlB, 0, m, &cVar_trSuFGlB_sendMessage);
  cMsg_xn8vU1TN_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_nu048CLv, 0, m, &cVar_nu048CLv_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_3JPv7bcq, 0, m, &cVar_3JPv7bcq_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_BdiVtWFA, 0, m, &cVar_BdiVtWFA_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_pkcTTgHR, 0, m, &cVar_pkcTTgHR_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_vXZZN7Wo, 0, m, &cVar_vXZZN7Wo_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_wuRMR9le, 0, m, &cVar_wuRMR9le_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_yeV5qANo, 0, m, &cVar_yeV5qANo_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_Hg9kPjvt, 0, m, &cVar_Hg9kPjvt_sendMessage);
  cMsg_QXRjl9VS_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_E23hfvrT, 0, m, &cVar_E23hfvrT_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_gHq9nltM, 0, m, &cVar_gHq9nltM_sendMessage);
  cMsg_8DNPKs6T_sendMessage(_c, 0, m);
  cMsg_oR989Oml_sendMessage(_c, 0, m);
}

void Heavy_prog::cReceive_hEXxY5BZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_wXpL7Lha, HV_BINOP_MULTIPLY, 0, m, &cBinop_wXpL7Lha_sendMessage);
}

void Heavy_prog::cReceive_jLGrvod3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_jLTUiXxx_sendMessage(_c, 0, m);
  cMsg_90cSfcQH_sendMessage(_c, 0, m);
  cMsg_FBNu9Dws_sendMessage(_c, 0, m);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -2.0f, 0, m, &cBinop_X0SgaKEZ_sendMessage);
}

void Heavy_prog::cReceive_UZ3q3013_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_GdlAPtXj_sendMessage);
  cMsg_WeF308Jw_sendMessage(_c, 0, m);
}

void Heavy_prog::cReceive_1uYiVQSR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_e5WuT1Hu, HV_BINOP_MULTIPLY, 0, m, &cBinop_e5WuT1Hu_sendMessage);
}

void Heavy_prog::cReceive_ujFLffTo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_rTn8mKNi_sendMessage(_c, 0, m);
  cMsg_6nyoY1Ml_sendMessage(_c, 0, m);
  cMsg_l4PPbctb_sendMessage(_c, 0, m);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -2.0f, 0, m, &cBinop_B31eEX7w_sendMessage);
}

void Heavy_prog::cReceive_RjUHK0UV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_ht5S2mWD_sendMessage);
  cMsg_bhl6EWEB_sendMessage(_c, 0, m);
}

void Heavy_prog::cReceive_zC0sXkWV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_8sJM8Vqs, 0, m, &cSlice_8sJM8Vqs_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_uvVB9JWN, 0, m, &cSlice_uvVB9JWN_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_aVtOtDdW, 0, m, &cSlice_aVtOtDdW_sendMessage);
}

void Heavy_prog::cReceive_iY2aKgYk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_cY26xX6b, 0, m, &cSlice_cY26xX6b_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_beD3LScr, 0, m, &cSlice_beD3LScr_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_0BzyKePF, 0, m, &cSlice_0BzyKePF_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_9cShn2pW, 0, m, &cSlice_9cShn2pW_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_nww5UdEW, 0, m, &cSlice_nww5UdEW_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_r5W61ffA, 0, m, &cSlice_r5W61ffA_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_9b1oKPAz, 0, m, &cSlice_9b1oKPAz_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_BhbzN3lY, 0, m, &cSlice_BhbzN3lY_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_LoH4l7Du, 0, m, &cSlice_LoH4l7Du_sendMessage);
}

void Heavy_prog::cReceive_2yg3PMZB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_IxJCHTdk, 0, m, &cSlice_IxJCHTdk_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_1VB5q5bI, 0, m, &cSlice_1VB5q5bI_sendMessage);
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
  hv_bufferf_t Bf0, Bf1, Bf2, Bf3, Bf4, Bf5, Bf6, Bf7;

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
    __hv_varread_f(&sVarf_55uGsKPW, VOf(Bf0));
    __hv_phasor_f(&sPhasor_8r9Gzpcz, VIf(Bf0), VOf(Bf1));
    __hv_add_f(VIf(Bf1), VIf(Bf1), VOf(Bf2));
    __hv_var_k_f(VOf(Bf3), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_sub_f(VIf(Bf2), VIf(Bf3), VOf(Bf3));
    __hv_varread_f(&sVarf_qTWDqhju, VOf(Bf2));
    __hv_mul_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_var_k_f(VOf(Bf0), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_div_f(VIf(Bf0), VIf(Bf2), VOf(Bf4));
    __hv_mul_f(VIf(Bf1), VIf(Bf4), VOf(Bf5));
    __hv_sub_f(VIf(Bf5), VIf(Bf0), VOf(Bf6));
    __hv_mul_f(VIf(Bf6), VIf(Bf6), VOf(Bf6));
    __hv_lte_f(VIf(Bf1), VIf(Bf2), VOf(Bf7));
    __hv_and_f(VIf(Bf6), VIf(Bf7), VOf(Bf7));
    __hv_sub_f(VIf(Bf4), VIf(Bf0), VOf(Bf4));
    __hv_sub_f(VIf(Bf5), VIf(Bf4), VOf(Bf4));
    __hv_mul_f(VIf(Bf4), VIf(Bf4), VOf(Bf4));
    __hv_sub_f(VIf(Bf0), VIf(Bf2), VOf(Bf2));
    __hv_gte_f(VIf(Bf1), VIf(Bf2), VOf(Bf2));
    __hv_and_f(VIf(Bf4), VIf(Bf2), VOf(Bf2));
    __hv_sub_f(VIf(Bf7), VIf(Bf2), VOf(Bf2));
    __hv_add_f(VIf(Bf3), VIf(Bf2), VOf(Bf2));
    __hv_line_f(&sLine_EfgfSfdV, VOf(Bf3));
    __hv_line_f(&sLine_lfViwZwu, VOf(Bf7));
    __hv_line_f(&sLine_W7Xbi5S5, VOf(Bf4));
    __hv_line_f(&sLine_sqsDTgvq, VOf(Bf1));
    __hv_line_f(&sLine_Tx0TGfLu, VOf(Bf0));
    __hv_biquad_f(&sBiquad_s_MqBlhoR5, VIf(Bf2), VIf(Bf3), VIf(Bf7), VIf(Bf4), VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_varread_f(&sVarf_SiNH5FLu, VOf(Bf1));
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf0), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O1), VOf(O1));
    __hv_varread_f(&sVarf_AFhXb2iL, VOf(Bf0));
    __hv_phasor_f(&sPhasor_INtIFWLT, VIf(Bf0), VOf(Bf1));
    __hv_add_f(VIf(Bf1), VIf(Bf1), VOf(Bf4));
    __hv_var_k_f(VOf(Bf7), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_sub_f(VIf(Bf4), VIf(Bf7), VOf(Bf7));
    __hv_varread_f(&sVarf_rg72z3nJ, VOf(Bf4));
    __hv_mul_f(VIf(Bf0), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf0), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_div_f(VIf(Bf0), VIf(Bf4), VOf(Bf3));
    __hv_mul_f(VIf(Bf1), VIf(Bf3), VOf(Bf2));
    __hv_sub_f(VIf(Bf2), VIf(Bf0), VOf(Bf5));
    __hv_mul_f(VIf(Bf5), VIf(Bf5), VOf(Bf5));
    __hv_lte_f(VIf(Bf1), VIf(Bf4), VOf(Bf6));
    __hv_and_f(VIf(Bf5), VIf(Bf6), VOf(Bf6));
    __hv_sub_f(VIf(Bf3), VIf(Bf0), VOf(Bf3));
    __hv_sub_f(VIf(Bf2), VIf(Bf3), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf3), VOf(Bf3));
    __hv_sub_f(VIf(Bf0), VIf(Bf4), VOf(Bf4));
    __hv_gte_f(VIf(Bf1), VIf(Bf4), VOf(Bf4));
    __hv_and_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_sub_f(VIf(Bf6), VIf(Bf4), VOf(Bf4));
    __hv_add_f(VIf(Bf7), VIf(Bf4), VOf(Bf4));
    __hv_line_f(&sLine_MOufmpiq, VOf(Bf7));
    __hv_line_f(&sLine_AHZMYFM8, VOf(Bf6));
    __hv_line_f(&sLine_OSGuqlCO, VOf(Bf3));
    __hv_line_f(&sLine_cSgka7VF, VOf(Bf1));
    __hv_line_f(&sLine_zGcTLAb4, VOf(Bf0));
    __hv_biquad_f(&sBiquad_s_gCeCZYC1, VIf(Bf4), VIf(Bf7), VIf(Bf6), VIf(Bf3), VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_varread_f(&sVarf_XUaSTwuF, VOf(Bf1));
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf0), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
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
