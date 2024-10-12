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
  numBytes += sPhasor_k_init(&sPhasor_5CRlF2CW, 100.0f, sampleRate);
  numBytes += sPhasor_init(&sPhasor_10gjXKQw, sampleRate);
  numBytes += cSlice_init(&cSlice_njOVaWgK, 2, 1);
  numBytes += cSlice_init(&cSlice_U8WB1UCQ, 1, 1);
  numBytes += cSlice_init(&cSlice_qqFMHHUq, 0, 1);
  numBytes += cVar_init_f(&cVar_UwltQTy4, 0.0f);
  numBytes += cIf_init(&cIf_jMSjSu0i, false);
  numBytes += cIf_init(&cIf_iq6xRPv9, false);
  numBytes += cIf_init(&cIf_8UDuVlHm, false);
  numBytes += cIf_init(&cIf_LDXrXIyF, false);
  numBytes += cIf_init(&cIf_vy6gKbVj, false);
  numBytes += cBinop_init(&cBinop_IHSnVLrZ, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_K5yeKJGb, 1.0f); // __pow
  numBytes += cVar_init_f(&cVar_LTIGsoul, 73.0f);
  numBytes += cIf_init(&cIf_h1z921W9, true);
  numBytes += cVar_init_f(&cVar_hFAMCEE0, 0.0f);
  numBytes += cIf_init(&cIf_Xv1UB8Ja, false);
  numBytes += cSlice_init(&cSlice_XDzM0tYa, 2, 1);
  numBytes += cSlice_init(&cSlice_foWZ5hiU, 1, 1);
  numBytes += cSlice_init(&cSlice_5BpTCnZB, 0, 1);
  numBytes += cIf_init(&cIf_Z4HmQZNe, false);
  numBytes += cIf_init(&cIf_hXwfGsZP, false);
  numBytes += cIf_init(&cIf_YXzjDoTv, false);
  numBytes += cIf_init(&cIf_fKZ4NFJR, true);
  numBytes += cIf_init(&cIf_glm8y1HH, true);
  numBytes += cIf_init(&cIf_lMYdWNGZ, true);
  numBytes += cIf_init(&cIf_x602hRfg, true);
  numBytes += cIf_init(&cIf_7C9DVNrF, false);
  numBytes += cIf_init(&cIf_BUWOvw7V, false);
  numBytes += cVar_init_f(&cVar_lPQaiRUN, 0.0f);
  numBytes += cBinop_init(&cBinop_bd8MGeY3, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_ErE8pj7i, 0.0f); // __eq
  numBytes += cVar_init_f(&cVar_IMjyIwUm, 72.0f);
  numBytes += cIf_init(&cIf_R11TuY2r, true);
  numBytes += cVar_init_f(&cVar_CAEGMdpq, 0.0f);
  numBytes += cIf_init(&cIf_PVq143Av, false);
  numBytes += cSlice_init(&cSlice_ld0oD4Ki, 2, 1);
  numBytes += cSlice_init(&cSlice_4ARHdTIQ, 1, 1);
  numBytes += cSlice_init(&cSlice_nVkY4asf, 0, 1);
  numBytes += cIf_init(&cIf_6g1OyEV6, false);
  numBytes += cIf_init(&cIf_1OAHz7Wk, false);
  numBytes += cIf_init(&cIf_pprPcucK, false);
  numBytes += cIf_init(&cIf_qBGWHvzM, true);
  numBytes += cIf_init(&cIf_kRQejlyA, true);
  numBytes += cIf_init(&cIf_mw6CNlzm, true);
  numBytes += cIf_init(&cIf_i2nFvnTB, true);
  numBytes += cIf_init(&cIf_trl8ek5k, false);
  numBytes += cIf_init(&cIf_ahSFRJkv, false);
  numBytes += cVar_init_f(&cVar_nlXBLGwp, 0.0f);
  numBytes += cBinop_init(&cBinop_ej1QzzP2, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_smOb0Z0Z, 0.0f); // __eq
  numBytes += sVarf_init(&sVarf_mR3DdIoA, 0.5f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_mAfI6IWY, 0.0f, 0.0f, false);
  
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
    case 0x41BE0F9C: { // __hv_ctlin
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_VWlwmaFh_sendMessage);
      break;
    }
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_x5weB8GE_sendMessage);
      break;
    }
    case 0x67E37CA3: { // __hv_notein
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_D3F2E1E0_sendMessage);
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


void Heavy_prog::cSlice_njOVaWgK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_J1oRIgoQ_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_U8WB1UCQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_LDXrXIyF, 0, m, &cIf_LDXrXIyF_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_iq6xRPv9, 0, m, &cIf_iq6xRPv9_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_qqFMHHUq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_vy6gKbVj, 0, m, &cIf_vy6gKbVj_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_8UDuVlHm, 0, m, &cIf_8UDuVlHm_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_UwltQTy4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_vzIrLOOq_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_jMSjSu0i, 0, m, &cIf_jMSjSu0i_sendMessage);
}

void Heavy_prog::cUnop_8ikoS4qA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_Aa2NERkf_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_IHSnVLrZ, HV_BINOP_EQ, 1, m, &cBinop_IHSnVLrZ_sendMessage);
}

void Heavy_prog::cUnop_OndsUuP3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_Aa2NERkf_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_IHSnVLrZ, HV_BINOP_EQ, 1, m, &cBinop_IHSnVLrZ_sendMessage);
}

void Heavy_prog::cIf_jMSjSu0i_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_OndsUuP3_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_8ikoS4qA_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_vzIrLOOq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_jMSjSu0i, 1, m, &cIf_jMSjSu0i_sendMessage);
}

void Heavy_prog::cIf_iq6xRPv9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_8UDuVlHm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_SVRKwMyT_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_LDXrXIyF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_vy6gKbVj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_SVRKwMyT_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_J1oRIgoQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_IHSnVLrZ, HV_BINOP_EQ, 0, m, &cBinop_IHSnVLrZ_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_Iw69LZOW_sendMessage);
}

void Heavy_prog::cBinop_Aa2NERkf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_UFJZQ7z1_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_wYsVwh6b_sendMessage);
}

void Heavy_prog::cBinop_IHSnVLrZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ZNhKfnnj_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_lKBRCHN9_sendMessage);
}

void Heavy_prog::cCast_lKBRCHN9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_vy6gKbVj, 1, m, &cIf_vy6gKbVj_sendMessage);
}

void Heavy_prog::cCast_ZNhKfnnj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_iq6xRPv9, 1, m, &cIf_iq6xRPv9_sendMessage);
}

void Heavy_prog::cCast_UFJZQ7z1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_LDXrXIyF, 1, m, &cIf_LDXrXIyF_sendMessage);
}

void Heavy_prog::cCast_wYsVwh6b_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_8UDuVlHm, 1, m, &cIf_8UDuVlHm_sendMessage);
}

void Heavy_prog::cBinop_Iw69LZOW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cBinop_cKeapj8V_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_mAfI6IWY, m);
}

void Heavy_prog::cBinop_SVRKwMyT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 12.0f, 0, m, &cBinop_NicfM6aA_sendMessage);
}

void Heavy_prog::cBinop_NicfM6aA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_K5yeKJGb, HV_BINOP_POW, 1, m, &cBinop_K5yeKJGb_sendMessage);
  cMsg_VxWDOwW0_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_K5yeKJGb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 440.0f, 0, m, &cBinop_cKeapj8V_sendMessage);
}

void Heavy_prog::cMsg_VxWDOwW0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 2.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_K5yeKJGb, HV_BINOP_POW, 0, m, &cBinop_K5yeKJGb_sendMessage);
}

void Heavy_prog::cVar_LTIGsoul_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_he1yGEbV_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_h1z921W9, 0, m, &cIf_h1z921W9_sendMessage);
}

void Heavy_prog::cUnop_2RX9G84t_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_UWaXKsab_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_caJit7ZZ_sendMessage);
}

void Heavy_prog::cUnop_U81NxalZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_UWaXKsab_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_caJit7ZZ_sendMessage);
}

void Heavy_prog::cIf_h1z921W9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_U81NxalZ_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_2RX9G84t_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_he1yGEbV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_h1z921W9, 1, m, &cIf_h1z921W9_sendMessage);
}

void Heavy_prog::cVar_hFAMCEE0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_48mPeFov_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_Xv1UB8Ja, 0, m, &cIf_Xv1UB8Ja_sendMessage);
}

void Heavy_prog::cUnop_kXIG7DiZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Yfgzc8jN_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_qn8Yx7ve_sendMessage);
}

void Heavy_prog::cUnop_lLAbQPuU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Yfgzc8jN_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_qn8Yx7ve_sendMessage);
}

void Heavy_prog::cIf_Xv1UB8Ja_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_lLAbQPuU_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_kXIG7DiZ_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_48mPeFov_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_Xv1UB8Ja, 1, m, &cIf_Xv1UB8Ja_sendMessage);
}

void Heavy_prog::cSlice_XDzM0tYa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_ulvY1sG3_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_foWZ5hiU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_hXwfGsZP, 0, m, &cIf_hXwfGsZP_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_bd8MGeY3, HV_BINOP_EQ, 0, m, &cBinop_bd8MGeY3_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_5BpTCnZB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_Z4HmQZNe, 0, m, &cIf_Z4HmQZNe_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_YXzjDoTv, 0, m, &cIf_YXzjDoTv_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_Z4HmQZNe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 127.0f, 0, m, &cBinop_pXSisbR6_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_hXwfGsZP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_YXzjDoTv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_glm8y1HH, 0, m, &cIf_glm8y1HH_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_fKZ4NFJR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_x602hRfg, 0, m, &cIf_x602hRfg_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_glm8y1HH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_lMYdWNGZ, 0, m, &cIf_lMYdWNGZ_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_lMYdWNGZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 127.0f, 0, m, &cBinop_pXSisbR6_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_x602hRfg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_7C9DVNrF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_ErE8pj7i, HV_BINOP_EQ, 0, m, &cBinop_ErE8pj7i_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_BUWOvw7V_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cVar_lPQaiRUN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_fKZ4NFJR, 0, m, &cIf_fKZ4NFJR_sendMessage);
}

void Heavy_prog::cBinop_ulvY1sG3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_Tg5b3aIt_sendMessage);
}

void Heavy_prog::cBinop_Tg5b3aIt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_N5wVmH5e_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_JAxu8FN0_sendMessage);
}

void Heavy_prog::cBinop_bd8MGeY3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_KkU5cQID_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_2wKdfXvc_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jpvSln7f_sendMessage);
}

void Heavy_prog::cCast_UWaXKsab_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_bd8MGeY3, HV_BINOP_EQ, 1, m, &cBinop_bd8MGeY3_sendMessage);
}

void Heavy_prog::cCast_caJit7ZZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_7ZIHwyOP_sendMessage);
}

void Heavy_prog::cBinop_7ZIHwyOP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Uq3DnYI7_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_QfSzJDHW_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_BtT4A8kl_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zrnerRPB_sendMessage);
}

void Heavy_prog::cBinop_Q5zHsRDk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_glm8y1HH, 1, m, &cIf_glm8y1HH_sendMessage);
}

void Heavy_prog::cCast_Yfgzc8jN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_5JwjQYK5_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_LtqTUSaS_sendMessage);
}

void Heavy_prog::cCast_qn8Yx7ve_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ErE8pj7i, HV_BINOP_EQ, 1, m, &cBinop_ErE8pj7i_sendMessage);
}

void Heavy_prog::cBinop_5JwjQYK5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_7C9DVNrF, 1, m, &cIf_7C9DVNrF_sendMessage);
}

void Heavy_prog::cCast_JAxu8FN0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hDHgTjQp_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_qhCSaerc_sendMessage);
}

void Heavy_prog::cCast_N5wVmH5e_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_7C9DVNrF, 0, m, &cIf_7C9DVNrF_sendMessage);
}

void Heavy_prog::cBinop_LtqTUSaS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_x602hRfg, 1, m, &cIf_x602hRfg_sendMessage);
}

void Heavy_prog::cBinop_ErE8pj7i_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_lMYdWNGZ, 1, m, &cIf_lMYdWNGZ_sendMessage);
}

void Heavy_prog::cCast_qhCSaerc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_lPQaiRUN, 1, m, &cVar_lPQaiRUN_sendMessage);
}

void Heavy_prog::cCast_hDHgTjQp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_BUWOvw7V, 0, m, &cIf_BUWOvw7V_sendMessage);
}

void Heavy_prog::cCast_jpvSln7f_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_YXzjDoTv, 1, m, &cIf_YXzjDoTv_sendMessage);
}

void Heavy_prog::cCast_2wKdfXvc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_lPQaiRUN, 0, m, &cVar_lPQaiRUN_sendMessage);
}

void Heavy_prog::cCast_KkU5cQID_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_fKZ4NFJR, 1, m, &cIf_fKZ4NFJR_sendMessage);
}

void Heavy_prog::cCast_zrnerRPB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_Q5zHsRDk_sendMessage);
}

void Heavy_prog::cCast_BtT4A8kl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_Z4HmQZNe, 1, m, &cIf_Z4HmQZNe_sendMessage);
}

void Heavy_prog::cCast_QfSzJDHW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_hXwfGsZP, 1, m, &cIf_hXwfGsZP_sendMessage);
}

void Heavy_prog::cCast_Uq3DnYI7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_BUWOvw7V, 1, m, &cIf_BUWOvw7V_sendMessage);
}

void Heavy_prog::cVar_IMjyIwUm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_4YsXndkp_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_R11TuY2r, 0, m, &cIf_R11TuY2r_sendMessage);
}

void Heavy_prog::cUnop_qAIU69XY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_DafZuTGZ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jIjrI1fO_sendMessage);
}

void Heavy_prog::cUnop_x3TVx2fE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_DafZuTGZ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jIjrI1fO_sendMessage);
}

void Heavy_prog::cIf_R11TuY2r_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_x3TVx2fE_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_qAIU69XY_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_4YsXndkp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_R11TuY2r, 1, m, &cIf_R11TuY2r_sendMessage);
}

void Heavy_prog::cVar_CAEGMdpq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_HTFSp23A_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_PVq143Av, 0, m, &cIf_PVq143Av_sendMessage);
}

void Heavy_prog::cUnop_kzlWM12n_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_WosmBbHC_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_yOTUEeGx_sendMessage);
}

void Heavy_prog::cUnop_ep6i56GT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_WosmBbHC_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_yOTUEeGx_sendMessage);
}

void Heavy_prog::cIf_PVq143Av_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_ep6i56GT_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_kzlWM12n_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_HTFSp23A_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_PVq143Av, 1, m, &cIf_PVq143Av_sendMessage);
}

void Heavy_prog::cSlice_ld0oD4Ki_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_LBL2w8ST_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_4ARHdTIQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_1OAHz7Wk, 0, m, &cIf_1OAHz7Wk_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_ej1QzzP2, HV_BINOP_EQ, 0, m, &cBinop_ej1QzzP2_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_nVkY4asf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_6g1OyEV6, 0, m, &cIf_6g1OyEV6_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_pprPcucK, 0, m, &cIf_pprPcucK_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_6g1OyEV6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_5CRlF2CW, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_1OAHz7Wk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_pprPcucK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_kRQejlyA, 0, m, &cIf_kRQejlyA_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_qBGWHvzM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_i2nFvnTB, 0, m, &cIf_i2nFvnTB_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_kRQejlyA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_mw6CNlzm, 0, m, &cIf_mw6CNlzm_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_mw6CNlzm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_5CRlF2CW, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_i2nFvnTB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_trl8ek5k_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_smOb0Z0Z, HV_BINOP_EQ, 0, m, &cBinop_smOb0Z0Z_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_ahSFRJkv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cVar_nlXBLGwp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_qBGWHvzM, 0, m, &cIf_qBGWHvzM_sendMessage);
}

void Heavy_prog::cBinop_LBL2w8ST_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_xxaSKGxu_sendMessage);
}

void Heavy_prog::cBinop_xxaSKGxu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_W7m5uEIo_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5qBBKbg0_sendMessage);
}

void Heavy_prog::cBinop_ej1QzzP2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_V0jEmo2s_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_h6TY7OQT_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_i7ByvO0e_sendMessage);
}

void Heavy_prog::cCast_DafZuTGZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ej1QzzP2, HV_BINOP_EQ, 1, m, &cBinop_ej1QzzP2_sendMessage);
}

void Heavy_prog::cCast_jIjrI1fO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_FQQk43Xc_sendMessage);
}

void Heavy_prog::cBinop_FQQk43Xc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_B5F13IP5_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_4GsbWKpy_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_S54S82bP_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_cuZUNL06_sendMessage);
}

void Heavy_prog::cBinop_SDytO6IM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_kRQejlyA, 1, m, &cIf_kRQejlyA_sendMessage);
}

void Heavy_prog::cCast_yOTUEeGx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_smOb0Z0Z, HV_BINOP_EQ, 1, m, &cBinop_smOb0Z0Z_sendMessage);
}

void Heavy_prog::cCast_WosmBbHC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_HCnD54FH_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_vBwRv8U6_sendMessage);
}

void Heavy_prog::cBinop_HCnD54FH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_trl8ek5k, 1, m, &cIf_trl8ek5k_sendMessage);
}

void Heavy_prog::cCast_W7m5uEIo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_trl8ek5k, 0, m, &cIf_trl8ek5k_sendMessage);
}

void Heavy_prog::cCast_5qBBKbg0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_8II9fJni_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_vw4UKnFb_sendMessage);
}

void Heavy_prog::cBinop_vBwRv8U6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_i2nFvnTB, 1, m, &cIf_i2nFvnTB_sendMessage);
}

void Heavy_prog::cBinop_smOb0Z0Z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_mw6CNlzm, 1, m, &cIf_mw6CNlzm_sendMessage);
}

void Heavy_prog::cCast_8II9fJni_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_ahSFRJkv, 0, m, &cIf_ahSFRJkv_sendMessage);
}

void Heavy_prog::cCast_vw4UKnFb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_nlXBLGwp, 1, m, &cVar_nlXBLGwp_sendMessage);
}

void Heavy_prog::cCast_h6TY7OQT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_nlXBLGwp, 0, m, &cVar_nlXBLGwp_sendMessage);
}

void Heavy_prog::cCast_V0jEmo2s_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_qBGWHvzM, 1, m, &cIf_qBGWHvzM_sendMessage);
}

void Heavy_prog::cCast_i7ByvO0e_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_pprPcucK, 1, m, &cIf_pprPcucK_sendMessage);
}

void Heavy_prog::cCast_B5F13IP5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_ahSFRJkv, 1, m, &cIf_ahSFRJkv_sendMessage);
}

void Heavy_prog::cCast_4GsbWKpy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_1OAHz7Wk, 1, m, &cIf_1OAHz7Wk_sendMessage);
}

void Heavy_prog::cCast_cuZUNL06_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_SDytO6IM_sendMessage);
}

void Heavy_prog::cCast_S54S82bP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_6g1OyEV6, 1, m, &cIf_6g1OyEV6_sendMessage);
}

void Heavy_prog::cBinop_pXSisbR6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_mR3DdIoA, m);
}

void Heavy_prog::cReceive_x5weB8GE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_UwltQTy4, 0, m, &cVar_UwltQTy4_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_hFAMCEE0, 0, m, &cVar_hFAMCEE0_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_LTIGsoul, 0, m, &cVar_LTIGsoul_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_CAEGMdpq, 0, m, &cVar_CAEGMdpq_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_IMjyIwUm, 0, m, &cVar_IMjyIwUm_sendMessage);
}

void Heavy_prog::cReceive_D3F2E1E0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_njOVaWgK, 0, m, &cSlice_njOVaWgK_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_U8WB1UCQ, 0, m, &cSlice_U8WB1UCQ_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_qqFMHHUq, 0, m, &cSlice_qqFMHHUq_sendMessage);
}

void Heavy_prog::cReceive_VWlwmaFh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_XDzM0tYa, 0, m, &cSlice_XDzM0tYa_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_foWZ5hiU, 0, m, &cSlice_foWZ5hiU_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_5BpTCnZB, 0, m, &cSlice_5BpTCnZB_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_ld0oD4Ki, 0, m, &cSlice_ld0oD4Ki_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_4ARHdTIQ, 0, m, &cSlice_4ARHdTIQ_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_nVkY4asf, 0, m, &cSlice_nVkY4asf_sendMessage);
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
  hv_bufferf_t Bf0, Bf1, Bf2, Bf3, Bf4;

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
    __hv_phasor_k_f(&sPhasor_5CRlF2CW, VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_abs_f(VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf0), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_mul_f(VIf(Bf1), VIf(Bf1), VOf(Bf0));
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf2));
    __hv_mul_f(VIf(Bf2), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf3), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf4), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf2), VIf(Bf4), VIf(Bf1), VOf(Bf1));
    __hv_fma_f(VIf(Bf0), VIf(Bf3), VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf3), 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f);
    __hv_varread_f(&sVarf_mAfI6IWY, VOf(Bf0));
    __hv_fma_f(VIf(Bf1), VIf(Bf3), VIf(Bf0), VOf(Bf0));
    __hv_phasor_f(&sPhasor_10gjXKQw, VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf3), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf0), VIf(Bf3), VOf(Bf3));
    __hv_abs_f(VIf(Bf3), VOf(Bf3));
    __hv_var_k_f(VOf(Bf0), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf3), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf3), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf0), VIf(Bf3), VOf(Bf3));
    __hv_mul_f(VIf(Bf3), VIf(Bf3), VOf(Bf0));
    __hv_mul_f(VIf(Bf3), VIf(Bf0), VOf(Bf1));
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf4), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf2), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf1), VIf(Bf2), VIf(Bf3), VOf(Bf3));
    __hv_fma_f(VIf(Bf0), VIf(Bf4), VIf(Bf3), VOf(Bf3));
    __hv_varread_f(&sVarf_mR3DdIoA, VOf(Bf4));
    __hv_mul_f(VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_add_f(VIf(Bf4), VIf(O0), VOf(O0));
    __hv_add_f(VIf(Bf4), VIf(O1), VOf(O1));

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
