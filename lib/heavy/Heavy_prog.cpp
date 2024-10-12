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
  numBytes += sPhasor_k_init(&sPhasor_H9u1rhpH, 200.0f, sampleRate);
  numBytes += cSlice_init(&cSlice_7FDdK6T1, 2, 1);
  numBytes += cSlice_init(&cSlice_maSKos7I, 1, 1);
  numBytes += cSlice_init(&cSlice_PUG57Jex, 0, 1);
  numBytes += cVar_init_f(&cVar_KUKY92vm, 0.0f);
  numBytes += cIf_init(&cIf_IlOOHw50, false);
  numBytes += cIf_init(&cIf_rmECsuW2, false);
  numBytes += cIf_init(&cIf_AigRLO9W, false);
  numBytes += cIf_init(&cIf_wdeNKQeu, false);
  numBytes += cIf_init(&cIf_5nvH5JCe, false);
  numBytes += cBinop_init(&cBinop_IGZfCQq7, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_eKVm5J1z, 1.0f); // __pow
  numBytes += cVar_init_f(&cVar_eogrP5YK, 73.0f);
  numBytes += cIf_init(&cIf_hLj0i5MA, true);
  numBytes += cVar_init_f(&cVar_5FQydvFo, 0.0f);
  numBytes += cIf_init(&cIf_hpfMkxNs, false);
  numBytes += cSlice_init(&cSlice_WaaNDK7e, 2, 1);
  numBytes += cSlice_init(&cSlice_0R5PYa4L, 1, 1);
  numBytes += cSlice_init(&cSlice_UPgDSKI5, 0, 1);
  numBytes += cIf_init(&cIf_sXfJLH8N, false);
  numBytes += cIf_init(&cIf_8drHcHwl, false);
  numBytes += cIf_init(&cIf_BljEUZR3, false);
  numBytes += cIf_init(&cIf_zpI80Znz, true);
  numBytes += cIf_init(&cIf_cz7p1Iv7, true);
  numBytes += cIf_init(&cIf_CdYUDEUx, true);
  numBytes += cIf_init(&cIf_v2GCU7nc, true);
  numBytes += cIf_init(&cIf_3Uff9V5B, false);
  numBytes += cIf_init(&cIf_FElZMpJf, false);
  numBytes += cVar_init_f(&cVar_0b43KBEW, 0.0f);
  numBytes += cBinop_init(&cBinop_terlue8e, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_Wq73laAg, 0.0f); // __eq
  numBytes += sVarf_init(&sVarf_f6CQApO6, 0.5f, 0.0f, false);
  
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
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_Js0vSq3Z_sendMessage);
      break;
    }
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_bxCS8sSh_sendMessage);
      break;
    }
    case 0x67E37CA3: { // __hv_notein
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_uGIFXRGu_sendMessage);
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


void Heavy_prog::cSlice_7FDdK6T1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_KZsFjOXd_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_maSKos7I_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_wdeNKQeu, 0, m, &cIf_wdeNKQeu_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_rmECsuW2, 0, m, &cIf_rmECsuW2_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_PUG57Jex_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_5nvH5JCe, 0, m, &cIf_5nvH5JCe_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_AigRLO9W, 0, m, &cIf_AigRLO9W_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_KUKY92vm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_J8X3jxKp_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_IlOOHw50, 0, m, &cIf_IlOOHw50_sendMessage);
}

void Heavy_prog::cUnop_Ndp0lOm6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_dxa0c7a9_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_IGZfCQq7, HV_BINOP_EQ, 1, m, &cBinop_IGZfCQq7_sendMessage);
}

void Heavy_prog::cUnop_C9JUA2Al_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_dxa0c7a9_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_IGZfCQq7, HV_BINOP_EQ, 1, m, &cBinop_IGZfCQq7_sendMessage);
}

void Heavy_prog::cIf_IlOOHw50_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_C9JUA2Al_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_Ndp0lOm6_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_J8X3jxKp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_IlOOHw50, 1, m, &cIf_IlOOHw50_sendMessage);
}

void Heavy_prog::cIf_rmECsuW2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_AigRLO9W_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_PjEL715h_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_wdeNKQeu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_5nvH5JCe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_PjEL715h_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_KZsFjOXd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_IGZfCQq7, HV_BINOP_EQ, 0, m, &cBinop_IGZfCQq7_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_IBKzrHiN_sendMessage);
}

void Heavy_prog::cBinop_dxa0c7a9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_nJgAcTxP_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_0tQ0xQ7a_sendMessage);
}

void Heavy_prog::cBinop_IGZfCQq7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_eTb8hngH_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ursitnSV_sendMessage);
}

void Heavy_prog::cCast_ursitnSV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_5nvH5JCe, 1, m, &cIf_5nvH5JCe_sendMessage);
}

void Heavy_prog::cCast_eTb8hngH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_rmECsuW2, 1, m, &cIf_rmECsuW2_sendMessage);
}

void Heavy_prog::cCast_nJgAcTxP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_wdeNKQeu, 1, m, &cIf_wdeNKQeu_sendMessage);
}

void Heavy_prog::cCast_0tQ0xQ7a_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_AigRLO9W, 1, m, &cIf_AigRLO9W_sendMessage);
}

void Heavy_prog::cBinop_IBKzrHiN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cBinop_1q8jwjGL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_H9u1rhpH, 0, m);
}

void Heavy_prog::cBinop_PjEL715h_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 12.0f, 0, m, &cBinop_KYjCCH1a_sendMessage);
}

void Heavy_prog::cBinop_KYjCCH1a_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_eKVm5J1z, HV_BINOP_POW, 1, m, &cBinop_eKVm5J1z_sendMessage);
  cMsg_a13eg3mz_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_eKVm5J1z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 440.0f, 0, m, &cBinop_1q8jwjGL_sendMessage);
}

void Heavy_prog::cMsg_a13eg3mz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 2.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_eKVm5J1z, HV_BINOP_POW, 0, m, &cBinop_eKVm5J1z_sendMessage);
}

void Heavy_prog::cVar_eogrP5YK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_BdxVhBeM_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_hLj0i5MA, 0, m, &cIf_hLj0i5MA_sendMessage);
}

void Heavy_prog::cUnop_fNi95GNr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_1WTAZX9W_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_H0RAP57B_sendMessage);
}

void Heavy_prog::cUnop_V9fFuZDQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_1WTAZX9W_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_H0RAP57B_sendMessage);
}

void Heavy_prog::cIf_hLj0i5MA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_V9fFuZDQ_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_fNi95GNr_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_BdxVhBeM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_hLj0i5MA, 1, m, &cIf_hLj0i5MA_sendMessage);
}

void Heavy_prog::cVar_5FQydvFo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_fSLlHbdK_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_hpfMkxNs, 0, m, &cIf_hpfMkxNs_sendMessage);
}

void Heavy_prog::cUnop_5rq4xJ1w_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_M6GCr5zU_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_3s2Cno8l_sendMessage);
}

void Heavy_prog::cUnop_em4jLeOM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_M6GCr5zU_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_3s2Cno8l_sendMessage);
}

void Heavy_prog::cIf_hpfMkxNs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_em4jLeOM_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_5rq4xJ1w_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_fSLlHbdK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_hpfMkxNs, 1, m, &cIf_hpfMkxNs_sendMessage);
}

void Heavy_prog::cSlice_WaaNDK7e_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_j3MVbJW5_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_0R5PYa4L_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_8drHcHwl, 0, m, &cIf_8drHcHwl_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_terlue8e, HV_BINOP_EQ, 0, m, &cBinop_terlue8e_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_UPgDSKI5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_sXfJLH8N, 0, m, &cIf_sXfJLH8N_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_BljEUZR3, 0, m, &cIf_BljEUZR3_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_sXfJLH8N_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 127.0f, 0, m, &cBinop_POchai4X_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_8drHcHwl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_BljEUZR3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_cz7p1Iv7, 0, m, &cIf_cz7p1Iv7_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_zpI80Znz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_v2GCU7nc, 0, m, &cIf_v2GCU7nc_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_cz7p1Iv7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_CdYUDEUx, 0, m, &cIf_CdYUDEUx_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_CdYUDEUx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 127.0f, 0, m, &cBinop_POchai4X_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_v2GCU7nc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_3Uff9V5B_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_Wq73laAg, HV_BINOP_EQ, 0, m, &cBinop_Wq73laAg_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_FElZMpJf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cVar_0b43KBEW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_zpI80Znz, 0, m, &cIf_zpI80Znz_sendMessage);
}

void Heavy_prog::cBinop_j3MVbJW5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_q7eY7fjH_sendMessage);
}

void Heavy_prog::cBinop_q7eY7fjH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jD1aoycG_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_fcLKgzzz_sendMessage);
}

void Heavy_prog::cBinop_terlue8e_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_4FHgelvL_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_SId5IZSd_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_LrnBkiud_sendMessage);
}

void Heavy_prog::cCast_H0RAP57B_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_lEGOlVWs_sendMessage);
}

void Heavy_prog::cCast_1WTAZX9W_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_terlue8e, HV_BINOP_EQ, 1, m, &cBinop_terlue8e_sendMessage);
}

void Heavy_prog::cBinop_lEGOlVWs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_JzNZu9oz_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_QZzOMGrb_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Yrq8jZWR_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5jCk77pU_sendMessage);
}

void Heavy_prog::cBinop_YOrG7TpK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_cz7p1Iv7, 1, m, &cIf_cz7p1Iv7_sendMessage);
}

void Heavy_prog::cCast_3s2Cno8l_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Wq73laAg, HV_BINOP_EQ, 1, m, &cBinop_Wq73laAg_sendMessage);
}

void Heavy_prog::cCast_M6GCr5zU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_ttYIzJgg_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_2KlGbzbc_sendMessage);
}

void Heavy_prog::cBinop_ttYIzJgg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_3Uff9V5B, 1, m, &cIf_3Uff9V5B_sendMessage);
}

void Heavy_prog::cCast_jD1aoycG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_3Uff9V5B, 0, m, &cIf_3Uff9V5B_sendMessage);
}

void Heavy_prog::cCast_fcLKgzzz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jb1dJY9G_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_vUsL5pOI_sendMessage);
}

void Heavy_prog::cBinop_2KlGbzbc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_v2GCU7nc, 1, m, &cIf_v2GCU7nc_sendMessage);
}

void Heavy_prog::cBinop_Wq73laAg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_CdYUDEUx, 1, m, &cIf_CdYUDEUx_sendMessage);
}

void Heavy_prog::cCast_vUsL5pOI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_0b43KBEW, 1, m, &cVar_0b43KBEW_sendMessage);
}

void Heavy_prog::cCast_jb1dJY9G_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_FElZMpJf, 0, m, &cIf_FElZMpJf_sendMessage);
}

void Heavy_prog::cCast_4FHgelvL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_zpI80Znz, 1, m, &cIf_zpI80Znz_sendMessage);
}

void Heavy_prog::cCast_LrnBkiud_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_BljEUZR3, 1, m, &cIf_BljEUZR3_sendMessage);
}

void Heavy_prog::cCast_SId5IZSd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_0b43KBEW, 0, m, &cVar_0b43KBEW_sendMessage);
}

void Heavy_prog::cCast_JzNZu9oz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_FElZMpJf, 1, m, &cIf_FElZMpJf_sendMessage);
}

void Heavy_prog::cCast_QZzOMGrb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_8drHcHwl, 1, m, &cIf_8drHcHwl_sendMessage);
}

void Heavy_prog::cCast_5jCk77pU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_YOrG7TpK_sendMessage);
}

void Heavy_prog::cCast_Yrq8jZWR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_sXfJLH8N, 1, m, &cIf_sXfJLH8N_sendMessage);
}

void Heavy_prog::cBinop_POchai4X_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_f6CQApO6, m);
}

void Heavy_prog::cReceive_bxCS8sSh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_KUKY92vm, 0, m, &cVar_KUKY92vm_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_5FQydvFo, 0, m, &cVar_5FQydvFo_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_eogrP5YK, 0, m, &cVar_eogrP5YK_sendMessage);
}

void Heavy_prog::cReceive_uGIFXRGu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_7FDdK6T1, 0, m, &cSlice_7FDdK6T1_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_maSKos7I, 0, m, &cSlice_maSKos7I_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_PUG57Jex, 0, m, &cSlice_PUG57Jex_sendMessage);
}

void Heavy_prog::cReceive_Js0vSq3Z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_WaaNDK7e, 0, m, &cSlice_WaaNDK7e_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_0R5PYa4L, 0, m, &cSlice_0R5PYa4L_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_UPgDSKI5, 0, m, &cSlice_UPgDSKI5_sendMessage);
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
    __hv_phasor_k_f(&sPhasor_H9u1rhpH, VOf(Bf0));
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
    __hv_varread_f(&sVarf_f6CQApO6, VOf(Bf3));
    __hv_mul_f(VIf(Bf1), VIf(Bf3), VOf(Bf3));
    __hv_add_f(VIf(Bf3), VIf(O1), VOf(O1));
    __hv_add_f(VIf(Bf3), VIf(O0), VOf(O0));

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
