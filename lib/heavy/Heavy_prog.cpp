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
  numBytes += sPhasor_init(&sPhasor_yQsb2nIb, sampleRate);
  numBytes += sLine_init(&sLine_wO3u3tui);
  numBytes += sLine_init(&sLine_Jr4j5thQ);
  numBytes += sLine_init(&sLine_KxpjAxuF);
  numBytes += sLine_init(&sLine_dR45bG2s);
  numBytes += sLine_init(&sLine_YsEKG2MS);
  numBytes += sBiquad_init(&sBiquad_s_egl1Ng4C);
  numBytes += sPhasor_init(&sPhasor_m2VEJZnO, sampleRate);
  numBytes += sLine_init(&sLine_h6bPVxoO);
  numBytes += sLine_init(&sLine_2kOE3uyS);
  numBytes += sLine_init(&sLine_NdyfbPvE);
  numBytes += sLine_init(&sLine_mkvCZZvv);
  numBytes += sLine_init(&sLine_26t4Armx);
  numBytes += sBiquad_init(&sBiquad_s_uPnadhNU);
  numBytes += cVar_init_f(&cVar_ryTE9SQW, 0.0f);
  numBytes += sVarf_init(&sVarf_u3GW6FJi, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_qfIIgGgn, 0.0f);
  numBytes += sVarf_init(&sVarf_e0srBG24, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_r4MmzX2O, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_B8vjFc42, 0.0f);
  numBytes += sVarf_init(&sVarf_aJl09ddZ, 0.0f, 0.0f, false);
  numBytes += cVar_init_f(&cVar_hhRSrzIT, 0.0f);
  numBytes += cBinop_init(&cBinop_duOsEG5f, 44100.0f); // __div
  numBytes += cBinop_init(&cBinop_zmR01dtl, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_9iJoKoDN, 0.0f);
  numBytes += cVar_init_f(&cVar_nE6V0vNq, 1000.0f);
  numBytes += cVar_init_f(&cVar_k5aEA6lZ, 4.0f);
  numBytes += cBinop_init(&cBinop_csEYG6Y8, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_ZDcQmKM0, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_bUcqh7kg, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_FsL6l3u7, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_kaCItd8m, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_KB2s82jw, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_23baWcpf, 44100.0f); // __div
  numBytes += cBinop_init(&cBinop_LQ4JEtpM, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_g1FMb36X, 0.0f);
  numBytes += cVar_init_f(&cVar_S1LcWebf, 1000.0f);
  numBytes += cVar_init_f(&cVar_Z8cQSFxp, 4.0f);
  numBytes += cBinop_init(&cBinop_6l6tsWEy, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_HGHfh6o0, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_5Y8uDTWI, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_rWj51Mfu, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_oy0yOcNA, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_PBmUccrf, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_KZ49bWkG, 0.0f);
  numBytes += cSlice_init(&cSlice_56I05L6n, 2, 1);
  numBytes += cSlice_init(&cSlice_fJKLxr87, 1, 1);
  numBytes += cSlice_init(&cSlice_UcBtKPmQ, 0, 1);
  numBytes += cVar_init_f(&cVar_L2Z4fpc9, 0.0f);
  numBytes += cIf_init(&cIf_9Swrtcgp, false);
  numBytes += cIf_init(&cIf_q4dgnYqS, false);
  numBytes += cIf_init(&cIf_kYKnEKod, false);
  numBytes += cIf_init(&cIf_vFLozoti, false);
  numBytes += cIf_init(&cIf_OTpmX9CC, false);
  numBytes += cBinop_init(&cBinop_iafQ0U8p, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_h6b2fJOx, 1.0f); // __pow
  numBytes += cVar_init_f(&cVar_kBYtz8Om, 42.0f);
  numBytes += cIf_init(&cIf_dKTiA6jo, true);
  numBytes += cVar_init_f(&cVar_PdHssghP, 0.0f);
  numBytes += cIf_init(&cIf_YHonjQ47, false);
  numBytes += cSlice_init(&cSlice_ML9hroip, 2, 1);
  numBytes += cSlice_init(&cSlice_ToVtZBP2, 1, 1);
  numBytes += cSlice_init(&cSlice_uDrgw2R4, 0, 1);
  numBytes += cIf_init(&cIf_usePjgmC, false);
  numBytes += cIf_init(&cIf_fJfAzJ58, false);
  numBytes += cIf_init(&cIf_HBWAAwww, false);
  numBytes += cIf_init(&cIf_m01msqUy, true);
  numBytes += cIf_init(&cIf_7Giwiys2, true);
  numBytes += cIf_init(&cIf_uBKbDpL5, true);
  numBytes += cIf_init(&cIf_FwmYYSFq, true);
  numBytes += cIf_init(&cIf_J7MitxgM, false);
  numBytes += cIf_init(&cIf_D53w4PBO, false);
  numBytes += cVar_init_f(&cVar_y42Fxe5u, 0.0f);
  numBytes += cBinop_init(&cBinop_CToj5eET, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_0pqCIpLd, 0.0f); // __eq
  numBytes += cVar_init_f(&cVar_UN9BtfWM, 43.0f);
  numBytes += cIf_init(&cIf_ij1jLCVz, true);
  numBytes += cVar_init_f(&cVar_YmwxP2GW, 0.0f);
  numBytes += cIf_init(&cIf_PdyPsZZA, false);
  numBytes += cSlice_init(&cSlice_MQagWwIB, 2, 1);
  numBytes += cSlice_init(&cSlice_MEyI9vgm, 1, 1);
  numBytes += cSlice_init(&cSlice_Mz9IADzw, 0, 1);
  numBytes += cIf_init(&cIf_cjosllpx, false);
  numBytes += cIf_init(&cIf_BbBN3Xdb, false);
  numBytes += cIf_init(&cIf_2rGQlO9G, false);
  numBytes += cIf_init(&cIf_tBmOUCOz, true);
  numBytes += cIf_init(&cIf_XVcXxDhN, true);
  numBytes += cIf_init(&cIf_bI2oXjHf, true);
  numBytes += cIf_init(&cIf_jSsNankV, true);
  numBytes += cIf_init(&cIf_sX4TYpdA, false);
  numBytes += cIf_init(&cIf_oS5japQ2, false);
  numBytes += cVar_init_f(&cVar_mT7drNaP, 0.0f);
  numBytes += cBinop_init(&cBinop_cdHhTG3r, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_QkuGfSQM, 0.0f); // __eq
  numBytes += cVar_init_f(&cVar_CU3FTxVb, 44.0f);
  numBytes += cIf_init(&cIf_TixPN7T5, true);
  numBytes += cVar_init_f(&cVar_pqKqEp8f, 0.0f);
  numBytes += cIf_init(&cIf_p2qq9c04, false);
  numBytes += cSlice_init(&cSlice_Rw8chYSl, 2, 1);
  numBytes += cSlice_init(&cSlice_rRoKs52Z, 1, 1);
  numBytes += cSlice_init(&cSlice_eozYHLox, 0, 1);
  numBytes += cIf_init(&cIf_G2HS1hDt, false);
  numBytes += cIf_init(&cIf_AP4W2tGn, false);
  numBytes += cIf_init(&cIf_SLkBtg8M, false);
  numBytes += cIf_init(&cIf_c7dtniuP, true);
  numBytes += cIf_init(&cIf_FWOe99S0, true);
  numBytes += cIf_init(&cIf_ONQDH4pC, true);
  numBytes += cIf_init(&cIf_mbt0fPu0, true);
  numBytes += cIf_init(&cIf_mvSxVdKn, false);
  numBytes += cIf_init(&cIf_w3QcwT7n, false);
  numBytes += cVar_init_f(&cVar_2uiQf6mR, 0.0f);
  numBytes += cBinop_init(&cBinop_X9r4Ovjp, 0.0f); // __eq
  numBytes += cBinop_init(&cBinop_VdsbSiTw, 0.0f); // __eq
  numBytes += cSlice_init(&cSlice_5qn8a93m, 1, 1);
  numBytes += cSlice_init(&cSlice_5y4CQ1SG, 0, 1);
  numBytes += cBinop_init(&cBinop_h346h3ME, 0.0f); // __sub
  numBytes += cBinop_init(&cBinop_WgEQlcV8, 0.0f); // __div
  numBytes += cIf_init(&cIf_hemXwpKW, false);
  numBytes += cVar_init_f(&cVar_FmsqelSG, 1.0f);
  numBytes += cVar_init_f(&cVar_HmcZEssK, 0.0f);
  numBytes += cVar_init_f(&cVar_nUtyzNav, 0.0f);
  numBytes += cDelay_init(this, &cDelay_WFAClOKM, 500.0f);
  numBytes += cIf_init(&cIf_Ud8Q5CmV, false);
  numBytes += cVar_init_f(&cVar_1Oa3asUj, 32.0f);
  numBytes += cVar_init_f(&cVar_wKBe7Ovk, 0.0f);
  numBytes += cVar_init_f(&cVar_4sL4NBXT, 0.0f);
  numBytes += cDelay_init(this, &cDelay_kqtLrc4a, 1.0f);
  numBytes += cVar_init_f(&cVar_RP75dAoa, 0.0f);
  numBytes += cVar_init_f(&cVar_V4bTJaJr, 0.0f);
  numBytes += cDelay_init(this, &cDelay_VX2x1n2f, 0.0f);
  numBytes += cVar_init_f(&cVar_sCwPzZi3, 20.0f);
  numBytes += cBinop_init(&cBinop_vIkdJ3MH, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_PAEnssjw, 0.0f);
  numBytes += cSlice_init(&cSlice_sIFKTEjy, 1, -1);
  numBytes += cSlice_init(&cSlice_WVO8No4n, 1, -1);
  numBytes += cVar_init_f(&cVar_5HnEabZn, 0.0f);
  numBytes += cVar_init_f(&cVar_E2pYkKTr, 20.0f);
  numBytes += cVar_init_f(&cVar_0JxmXT6k, 0.0f);
  numBytes += cVar_init_f(&cVar_RgJv1kIW, 0.0f);
  numBytes += cVar_init_f(&cVar_bgSaBrGb, 0.0f);
  numBytes += cSlice_init(&cSlice_pPsGdLJ9, 1, 1);
  numBytes += cSlice_init(&cSlice_4L6qFKQr, 0, 1);
  numBytes += cBinop_init(&cBinop_PdZuTyNT, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_RRRzi83x, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_JmRjXbmc, 0.0f); // __sub
  numBytes += cBinop_init(&cBinop_MgNdjdBF, 0.0f); // __add
  numBytes += cBinop_init(&cBinop_a00iGGhj, 20.0f); // __div
  numBytes += cBinop_init(&cBinop_QDZYi7ql, 0.0f); // __div
  numBytes += cBinop_init(&cBinop_arv6taHI, 0.0f); // __add
  numBytes += cBinop_init(&cBinop_uNP7rWZx, 0.0f); // __sub
  numBytes += cSlice_init(&cSlice_DfcbhiHe, 1, -1);
  numBytes += cBinop_init(&cBinop_Bz7qHPTi, 2.0f); // __sub
  numBytes += cBinop_init(&cBinop_HjgLSHyf, 2.0f); // __add
  numBytes += sVarf_init(&sVarf_uGfFQ7KK, 0.0f, 0.0f, false);
  numBytes += sVarf_init(&sVarf_HBUbZLMB, 0.0f, 0.0f, false);
  
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
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_96myYRUQ_sendMessage);
      break;
    }
    case 0x6F9D23C1: { // 1062-wcos
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_sZeJnywv_sendMessage);
      break;
    }
    case 0xD4FCB382: { // 1062-wsin
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_Qztg4E12_sendMessage);
      break;
    }
    case 0xEE0A93FE: { // 1103-alpha
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_VH4sTuxJ_sendMessage);
      break;
    }
    case 0x729D4D39: { // 1103-wcos
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_9Wj8ua1u_sendMessage);
      break;
    }
    case 0x4068BFB3: { // 1103-wsin
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_2xUMxCfG_sendMessage);
      break;
    }
    case 0x41BE0F9C: { // __hv_ctlin
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_76ajcvoG_sendMessage);
      break;
    }
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_3uCiN09j_sendMessage);
      break;
    }
    case 0x6FFF0BCF: { // __hv_midirealtimein
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_nIi8cd76_sendMessage);
      break;
    }
    case 0x67E37CA3: { // __hv_notein
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_BbyYEiN5_sendMessage);
      break;
    }
    case 0x3861AA6D: { // detune
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_pdHj3YN1_sendMessage);
      break;
    }
    case 0x345FC008: { // freq
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_Z5DvwMdG_sendMessage);
      break;
    }
    case 0x204E6574: { // oscFreq
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_TW0sjGKd_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_prog::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      case 0: {
        info->name = "detune";
        info->hash = 0x3861AA6D;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 127.0f;
        info->defaultVal = 1.0f;
        break;
      }
      case 1: {
        info->name = "freq";
        info->hash = 0x345FC008;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 50.0f;
        info->maxVal = 2000.0f;
        info->defaultVal = 500.0f;
        break;
      }
      case 2: {
        info->name = "oscFreq";
        info->hash = 0x204E6574;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 40.0f;
        info->maxVal = 400.0f;
        info->defaultVal = 80.0f;
        break;
      }
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
  return 3;
}



/*
 * Send Function Implementations
 */


void Heavy_prog::cVar_ryTE9SQW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_R3zudfjE_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_0W6AYdU0_sendMessage);
}

void Heavy_prog::cVar_qfIIgGgn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_onMessage(_c, &Context(_c)->sPhasor_m2VEJZnO, 1, m);
}

void Heavy_prog::cMsg_HIB95DBA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_KAM2X3oZ_sendMessage);
}

void Heavy_prog::cSystem_KAM2X3oZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_yBpdpZiA_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_yBpdpZiA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_rhrgu9YQ_sendMessage);
}

void Heavy_prog::cBinop_rhrgu9YQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_e0srBG24, m);
}

void Heavy_prog::cVar_B8vjFc42_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_onMessage(_c, &Context(_c)->sPhasor_yQsb2nIb, 1, m);
}

void Heavy_prog::cMsg_UDmcWmgT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_WgpeDiSC_sendMessage);
}

void Heavy_prog::cSystem_WgpeDiSC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_LpThHcQx_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_LpThHcQx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_tTClnEgh_sendMessage);
}

void Heavy_prog::cBinop_tTClnEgh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_aJl09ddZ, m);
}

void Heavy_prog::cVar_hhRSrzIT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 127.0f, 0, m, &cBinop_sp3QIAbc_sendMessage);
}

void Heavy_prog::cMsg_yJ6KhCHe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_wO3u3tui, 0, m, NULL);
}

void Heavy_prog::cMsg_ZIKUaoSO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_Jr4j5thQ, 0, m, NULL);
}

void Heavy_prog::cMsg_iT04GsnA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_KxpjAxuF, 0, m, NULL);
}

void Heavy_prog::cMsg_uIzKY47O_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_dR45bG2s, 0, m, NULL);
}

void Heavy_prog::cMsg_sFpHsvLl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_YsEKG2MS, 0, m, NULL);
}

void Heavy_prog::cMsg_PJJ1322p_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_7Dc6OVpa_sendMessage);
}

void Heavy_prog::cSystem_7Dc6OVpa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_duOsEG5f, HV_BINOP_DIVIDE, 1, m, &cBinop_duOsEG5f_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_YuLDy9oA_sendMessage);
}

void Heavy_prog::cUnop_7nM3lzsg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_DSCngXZm_sendMessage);
}

void Heavy_prog::cMsg_pQOXrbps_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cUnop_onMessage(_c, HV_UNOP_ATAN, m, &cUnop_7nM3lzsg_sendMessage);
}

void Heavy_prog::cBinop_DSCngXZm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_duOsEG5f, HV_BINOP_DIVIDE, 0, m, &cBinop_duOsEG5f_sendMessage);
}

void Heavy_prog::cCast_YuLDy9oA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_pQOXrbps_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_duOsEG5f_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_zmR01dtl, HV_BINOP_MULTIPLY, 1, m, &cBinop_zmR01dtl_sendMessage);
}

void Heavy_prog::cBinop_zmR01dtl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_9zuXc68R_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5ZTMGuIj_sendMessage);
}

void Heavy_prog::cUnop_rMnCtVzO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_0UJvkAx7_sendMessage(_c, 0, m);
}

void Heavy_prog::cUnop_CVHcLZTF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_6bti4t3Z_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_TuG4gRU1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 20.0f, 0, m, &cBinop_rnLMx4zR_sendMessage);
}

void Heavy_prog::cBinop_rnLMx4zR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_zmR01dtl, HV_BINOP_MULTIPLY, 0, m, &cBinop_zmR01dtl_sendMessage);
}

void Heavy_prog::cBinop_QahAJOgy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.1f, 0, m, &cBinop_rCkjDrfy_sendMessage);
}

void Heavy_prog::cBinop_rCkjDrfy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_mpg73U6z_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_NNwHBA2U_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ZDcQmKM0, HV_BINOP_MULTIPLY, 1, m, &cBinop_ZDcQmKM0_sendMessage);
}

void Heavy_prog::cMsg_4kWurzSp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_MeEXjTsp_sendMessage);
}

void Heavy_prog::cBinop_MeEXjTsp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_NNwHBA2U_sendMessage);
}

void Heavy_prog::cMsg_LrArsgSJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_EBb62hvA_sendMessage);
}

void Heavy_prog::cBinop_EBb62hvA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_bUcqh7kg, HV_BINOP_MULTIPLY, 1, m, &cBinop_bUcqh7kg_sendMessage);
}

void Heavy_prog::cBinop_w7qCgcuU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_FsL6l3u7, HV_BINOP_MULTIPLY, 1, m, &cBinop_FsL6l3u7_sendMessage);
}

void Heavy_prog::cMsg_yVce42d4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_alX5tRi0_sendMessage);
}

void Heavy_prog::cBinop_alX5tRi0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_w7qCgcuU_sendMessage);
}

void Heavy_prog::cBinop_dKEfjQWo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_9iJoKoDN, 1, m, &cVar_9iJoKoDN_sendMessage);
}

void Heavy_prog::cBinop_G9icV9sA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_kaCItd8m, HV_BINOP_MULTIPLY, 1, m, &cBinop_kaCItd8m_sendMessage);
}

void Heavy_prog::cMsg_7Oe4Vvc2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_zZkDYSPA_sendMessage);
}

void Heavy_prog::cBinop_zZkDYSPA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_KB2s82jw, HV_BINOP_MULTIPLY, 1, m, &cBinop_KB2s82jw_sendMessage);
}

void Heavy_prog::cVar_9iJoKoDN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_1QisVuQV_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_nE6V0vNq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_58a1BxBS_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 20000.0f, 0, m, &cBinop_TuG4gRU1_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_UblPeu42_sendMessage);
}

void Heavy_prog::cVar_k5aEA6lZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 100.0f, 0, m, &cBinop_QahAJOgy_sendMessage);
}

void Heavy_prog::cCast_5ZTMGuIj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_COS, m, &cUnop_CVHcLZTF_sendMessage);
}

void Heavy_prog::cCast_9zuXc68R_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_SIN, m, &cUnop_rMnCtVzO_sendMessage);
}

void Heavy_prog::cSend_0UJvkAx7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_Qztg4E12_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_6bti4t3Z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_sZeJnywv_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_FRVZDb6Y_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_96myYRUQ_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_mpg73U6z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_d1PMnZA2_sendMessage);
}

void Heavy_prog::cBinop_d1PMnZA2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_csEYG6Y8, HV_BINOP_MULTIPLY, 1, m, &cBinop_csEYG6Y8_sendMessage);
}

void Heavy_prog::cBinop_csEYG6Y8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_fDuNFjQQ_sendMessage);
}

void Heavy_prog::cBinop_fDuNFjQQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_FRVZDb6Y_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_1QisVuQV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_9joQmyyG_sendMessage);
}

void Heavy_prog::cBinop_9joQmyyG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_tppkClog_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_cx2WiLJT_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_8dvCXpP3_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zc4PreZn_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_4LDSIcLi_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_PitweSu2_sendMessage);
}

void Heavy_prog::cBinop_ZDcQmKM0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_yJ6KhCHe_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_bUcqh7kg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_ZIKUaoSO_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_FsL6l3u7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_iT04GsnA_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_kaCItd8m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_uIzKY47O_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_KB2s82jw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_sFpHsvLl_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_ERYAHtWJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_nE6V0vNq, 0, m, &cVar_nE6V0vNq_sendMessage);
}

void Heavy_prog::cCast_ThIWv3qH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_k5aEA6lZ, 1, m, &cVar_k5aEA6lZ_sendMessage);
}

void Heavy_prog::cCast_zc4PreZn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_FsL6l3u7, HV_BINOP_MULTIPLY, 0, m, &cBinop_FsL6l3u7_sendMessage);
}

void Heavy_prog::cCast_tppkClog_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_PitweSu2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_ZDcQmKM0, HV_BINOP_MULTIPLY, 0, m, &cBinop_ZDcQmKM0_sendMessage);
}

void Heavy_prog::cCast_4LDSIcLi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_bUcqh7kg, HV_BINOP_MULTIPLY, 0, m, &cBinop_bUcqh7kg_sendMessage);
}

void Heavy_prog::cCast_cx2WiLJT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_KB2s82jw, HV_BINOP_MULTIPLY, 0, m, &cBinop_KB2s82jw_sendMessage);
}

void Heavy_prog::cCast_8dvCXpP3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_kaCItd8m, HV_BINOP_MULTIPLY, 0, m, &cBinop_kaCItd8m_sendMessage);
}

void Heavy_prog::cCast_UblPeu42_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_9iJoKoDN, 0, m, &cVar_9iJoKoDN_sendMessage);
}

void Heavy_prog::cCast_58a1BxBS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_k5aEA6lZ, 0, m, &cVar_k5aEA6lZ_sendMessage);
}

void Heavy_prog::cMsg_cGoCU7Fd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_h6bPVxoO, 0, m, NULL);
}

void Heavy_prog::cMsg_iU8AnlhX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_2kOE3uyS, 0, m, NULL);
}

void Heavy_prog::cMsg_9UVgbG1b_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_NdyfbPvE, 0, m, NULL);
}

void Heavy_prog::cMsg_b2GKfMJh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_mkvCZZvv, 0, m, NULL);
}

void Heavy_prog::cMsg_CqL4H3Qi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 10.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_26t4Armx, 0, m, NULL);
}

void Heavy_prog::cMsg_FPWg0ceu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_eDQSu8QI_sendMessage);
}

void Heavy_prog::cSystem_eDQSu8QI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_23baWcpf, HV_BINOP_DIVIDE, 1, m, &cBinop_23baWcpf_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_oBWMPC86_sendMessage);
}

void Heavy_prog::cUnop_tOl4YrjU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 8.0f, 0, m, &cBinop_ZjKRWXkJ_sendMessage);
}

void Heavy_prog::cMsg_ZMRaMbPY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cUnop_onMessage(_c, HV_UNOP_ATAN, m, &cUnop_tOl4YrjU_sendMessage);
}

void Heavy_prog::cBinop_ZjKRWXkJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_23baWcpf, HV_BINOP_DIVIDE, 0, m, &cBinop_23baWcpf_sendMessage);
}

void Heavy_prog::cCast_oBWMPC86_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_ZMRaMbPY_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_23baWcpf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_LQ4JEtpM, HV_BINOP_MULTIPLY, 1, m, &cBinop_LQ4JEtpM_sendMessage);
}

void Heavy_prog::cBinop_LQ4JEtpM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_V23fTGkK_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_JutUWKty_sendMessage);
}

void Heavy_prog::cUnop_tj1nRzDO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_QiIUuGAP_sendMessage(_c, 0, m);
}

void Heavy_prog::cUnop_ghNkILuc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_9WsIDb92_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_O1eMbIfo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 20.0f, 0, m, &cBinop_MXvyqfoc_sendMessage);
}

void Heavy_prog::cBinop_MXvyqfoc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_LQ4JEtpM, HV_BINOP_MULTIPLY, 0, m, &cBinop_LQ4JEtpM_sendMessage);
}

void Heavy_prog::cBinop_Urc6bZRX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 0.1f, 0, m, &cBinop_JwA17CrT_sendMessage);
}

void Heavy_prog::cBinop_JwA17CrT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_MQauzLH7_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_XaFhYLQm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_HGHfh6o0, HV_BINOP_MULTIPLY, 1, m, &cBinop_HGHfh6o0_sendMessage);
}

void Heavy_prog::cMsg_vkDwq3MS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_Jqzm1Gw4_sendMessage);
}

void Heavy_prog::cBinop_Jqzm1Gw4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_XaFhYLQm_sendMessage);
}

void Heavy_prog::cMsg_POgAUptY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_gWr9DBNH_sendMessage);
}

void Heavy_prog::cBinop_gWr9DBNH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_5Y8uDTWI, HV_BINOP_MULTIPLY, 1, m, &cBinop_5Y8uDTWI_sendMessage);
}

void Heavy_prog::cBinop_T6wQMUqO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_rWj51Mfu, HV_BINOP_MULTIPLY, 1, m, &cBinop_rWj51Mfu_sendMessage);
}

void Heavy_prog::cMsg_hE3Lv4Zj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_7mqAJ60S_sendMessage);
}

void Heavy_prog::cBinop_7mqAJ60S_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_T6wQMUqO_sendMessage);
}

void Heavy_prog::cBinop_QUtbNYXe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_g1FMb36X, 1, m, &cVar_g1FMb36X_sendMessage);
}

void Heavy_prog::cBinop_SFfuAuyv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_oy0yOcNA, HV_BINOP_MULTIPLY, 1, m, &cBinop_oy0yOcNA_sendMessage);
}

void Heavy_prog::cMsg_zVy88w3J_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 0.0f, 0, m, &cBinop_DJwHmsqG_sendMessage);
}

void Heavy_prog::cBinop_DJwHmsqG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_PBmUccrf, HV_BINOP_MULTIPLY, 1, m, &cBinop_PBmUccrf_sendMessage);
}

void Heavy_prog::cVar_g1FMb36X_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_gw40at0h_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_S1LcWebf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_xUMkbUbd_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 20000.0f, 0, m, &cBinop_O1eMbIfo_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_mb6MWesV_sendMessage);
}

void Heavy_prog::cVar_Z8cQSFxp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MIN, 100.0f, 0, m, &cBinop_Urc6bZRX_sendMessage);
}

void Heavy_prog::cCast_V23fTGkK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_SIN, m, &cUnop_tj1nRzDO_sendMessage);
}

void Heavy_prog::cCast_JutUWKty_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_COS, m, &cUnop_ghNkILuc_sendMessage);
}

void Heavy_prog::cSend_QiIUuGAP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_2xUMxCfG_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_9WsIDb92_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_9Wj8ua1u_sendMessage(_c, 0, m);
}

void Heavy_prog::cSend_I9vvSQra_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cReceive_VH4sTuxJ_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_MQauzLH7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_rusYI2yi_sendMessage);
}

void Heavy_prog::cBinop_rusYI2yi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_6l6tsWEy, HV_BINOP_MULTIPLY, 1, m, &cBinop_6l6tsWEy_sendMessage);
}

void Heavy_prog::cBinop_6l6tsWEy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 0.5f, 0, m, &cBinop_9hWPc9hp_sendMessage);
}

void Heavy_prog::cBinop_9hWPc9hp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_I9vvSQra_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_gw40at0h_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_J83bKGeG_sendMessage);
}

void Heavy_prog::cBinop_J83bKGeG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_cd6AmpqH_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_LzJgXoGi_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_bDlvEkdb_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Aju1IYr1_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_I1bPZqDS_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Ih5Li0yO_sendMessage);
}

void Heavy_prog::cBinop_HGHfh6o0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_cGoCU7Fd_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_5Y8uDTWI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_iU8AnlhX_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_rWj51Mfu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_9UVgbG1b_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_oy0yOcNA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_b2GKfMJh_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_PBmUccrf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_CqL4H3Qi_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_ZiB32ign_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_S1LcWebf, 0, m, &cVar_S1LcWebf_sendMessage);
}

void Heavy_prog::cCast_vM7AdSEP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_Z8cQSFxp, 1, m, &cVar_Z8cQSFxp_sendMessage);
}

void Heavy_prog::cCast_cd6AmpqH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_I1bPZqDS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_5Y8uDTWI, HV_BINOP_MULTIPLY, 0, m, &cBinop_5Y8uDTWI_sendMessage);
}

void Heavy_prog::cCast_bDlvEkdb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_oy0yOcNA, HV_BINOP_MULTIPLY, 0, m, &cBinop_oy0yOcNA_sendMessage);
}

void Heavy_prog::cCast_Aju1IYr1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_rWj51Mfu, HV_BINOP_MULTIPLY, 0, m, &cBinop_rWj51Mfu_sendMessage);
}

void Heavy_prog::cCast_Ih5Li0yO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_HGHfh6o0, HV_BINOP_MULTIPLY, 0, m, &cBinop_HGHfh6o0_sendMessage);
}

void Heavy_prog::cCast_LzJgXoGi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_PBmUccrf, HV_BINOP_MULTIPLY, 0, m, &cBinop_PBmUccrf_sendMessage);
}

void Heavy_prog::cCast_xUMkbUbd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_Z8cQSFxp, 0, m, &cVar_Z8cQSFxp_sendMessage);
}

void Heavy_prog::cCast_mb6MWesV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_g1FMb36X, 0, m, &cVar_g1FMb36X_sendMessage);
}

void Heavy_prog::cVar_KZ49bWkG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 10.0f, 0, m, &cBinop_4N6SktEw_sendMessage);
}

void Heavy_prog::cSlice_56I05L6n_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_mYhSWwzz_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_fJKLxr87_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_vFLozoti, 0, m, &cIf_vFLozoti_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_q4dgnYqS, 0, m, &cIf_q4dgnYqS_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_UcBtKPmQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_OTpmX9CC, 0, m, &cIf_OTpmX9CC_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_kYKnEKod, 0, m, &cIf_kYKnEKod_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_L2Z4fpc9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_DE6Jz1C6_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_9Swrtcgp, 0, m, &cIf_9Swrtcgp_sendMessage);
}

void Heavy_prog::cUnop_lZRC0tUI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_P3RFwxNW_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_iafQ0U8p, HV_BINOP_EQ, 1, m, &cBinop_iafQ0U8p_sendMessage);
}

void Heavy_prog::cUnop_ZSTPQpg8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_P3RFwxNW_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_iafQ0U8p, HV_BINOP_EQ, 1, m, &cBinop_iafQ0U8p_sendMessage);
}

void Heavy_prog::cIf_9Swrtcgp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_ZSTPQpg8_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_lZRC0tUI_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_DE6Jz1C6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_9Swrtcgp, 1, m, &cIf_9Swrtcgp_sendMessage);
}

void Heavy_prog::cIf_q4dgnYqS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_kYKnEKod_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_uswKoylP_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_vFLozoti_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_OTpmX9CC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_uswKoylP_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_mYhSWwzz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_iafQ0U8p, HV_BINOP_EQ, 0, m, &cBinop_iafQ0U8p_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_IT0by13j_sendMessage);
}

void Heavy_prog::cBinop_P3RFwxNW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_VvJdJZzZ_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_T7zBx4Nu_sendMessage);
}

void Heavy_prog::cBinop_iafQ0U8p_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_dVwj9ANY_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_db9SGKpN_sendMessage);
}

void Heavy_prog::cCast_db9SGKpN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_OTpmX9CC, 1, m, &cIf_OTpmX9CC_sendMessage);
}

void Heavy_prog::cCast_dVwj9ANY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_q4dgnYqS, 1, m, &cIf_q4dgnYqS_sendMessage);
}

void Heavy_prog::cCast_T7zBx4Nu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_kYKnEKod, 1, m, &cIf_kYKnEKod_sendMessage);
}

void Heavy_prog::cCast_VvJdJZzZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_vFLozoti, 1, m, &cIf_vFLozoti_sendMessage);
}

void Heavy_prog::cBinop_IT0by13j_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cBinop_RqFUKNBz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_ryTE9SQW, 0, m, &cVar_ryTE9SQW_sendMessage);
}

void Heavy_prog::cBinop_uswKoylP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 12.0f, 0, m, &cBinop_zHChEKhi_sendMessage);
}

void Heavy_prog::cBinop_zHChEKhi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_h6b2fJOx, HV_BINOP_POW, 1, m, &cBinop_h6b2fJOx_sendMessage);
  cMsg_bT5wdYPj_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_h6b2fJOx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 440.0f, 0, m, &cBinop_RqFUKNBz_sendMessage);
}

void Heavy_prog::cMsg_bT5wdYPj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 2.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_h6b2fJOx, HV_BINOP_POW, 0, m, &cBinop_h6b2fJOx_sendMessage);
}

void Heavy_prog::cVar_kBYtz8Om_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_2r1PUB0J_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_dKTiA6jo, 0, m, &cIf_dKTiA6jo_sendMessage);
}

void Heavy_prog::cUnop_U0tziIMb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_k4HVKYx6_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_bJnJhvt0_sendMessage);
}

void Heavy_prog::cUnop_OTOwgdma_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_k4HVKYx6_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_bJnJhvt0_sendMessage);
}

void Heavy_prog::cIf_dKTiA6jo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_OTOwgdma_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_U0tziIMb_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_2r1PUB0J_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_dKTiA6jo, 1, m, &cIf_dKTiA6jo_sendMessage);
}

void Heavy_prog::cVar_PdHssghP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_CywOFLUv_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_YHonjQ47, 0, m, &cIf_YHonjQ47_sendMessage);
}

void Heavy_prog::cUnop_TkMmK1LJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_bfMzhA1V_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hnfYGk1e_sendMessage);
}

void Heavy_prog::cUnop_otTdWA7z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_bfMzhA1V_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hnfYGk1e_sendMessage);
}

void Heavy_prog::cIf_YHonjQ47_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_otTdWA7z_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_TkMmK1LJ_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_CywOFLUv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_YHonjQ47, 1, m, &cIf_YHonjQ47_sendMessage);
}

void Heavy_prog::cSlice_ML9hroip_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_3NoGDk1l_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_ToVtZBP2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_fJfAzJ58, 0, m, &cIf_fJfAzJ58_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_CToj5eET, HV_BINOP_EQ, 0, m, &cBinop_CToj5eET_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_uDrgw2R4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_usePjgmC, 0, m, &cIf_usePjgmC_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_HBWAAwww, 0, m, &cIf_HBWAAwww_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_usePjgmC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 3.0f, 0, m, &cBinop_53kk02Ay_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_fJfAzJ58_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_HBWAAwww_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_7Giwiys2, 0, m, &cIf_7Giwiys2_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_m01msqUy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_FwmYYSFq, 0, m, &cIf_FwmYYSFq_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_7Giwiys2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_uBKbDpL5, 0, m, &cIf_uBKbDpL5_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_uBKbDpL5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 3.0f, 0, m, &cBinop_53kk02Ay_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_FwmYYSFq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_J7MitxgM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_0pqCIpLd, HV_BINOP_EQ, 0, m, &cBinop_0pqCIpLd_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_D53w4PBO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cVar_y42Fxe5u_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_m01msqUy, 0, m, &cIf_m01msqUy_sendMessage);
}

void Heavy_prog::cBinop_3NoGDk1l_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_JKsLYhiv_sendMessage);
}

void Heavy_prog::cBinop_JKsLYhiv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Hk6dcXL4_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5Y9i65KB_sendMessage);
}

void Heavy_prog::cBinop_CToj5eET_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_CuT3M1Wr_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_pBSMvPsu_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_PsDu1dS1_sendMessage);
}

void Heavy_prog::cCast_bJnJhvt0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_rqiNhOIc_sendMessage);
}

void Heavy_prog::cCast_k4HVKYx6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_CToj5eET, HV_BINOP_EQ, 1, m, &cBinop_CToj5eET_sendMessage);
}

void Heavy_prog::cBinop_rqiNhOIc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xE0v3whT_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_dQ00dvVK_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_8HcwxaMl_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_t2jJhxCn_sendMessage);
}

void Heavy_prog::cBinop_J1sQKDP8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_7Giwiys2, 1, m, &cIf_7Giwiys2_sendMessage);
}

void Heavy_prog::cCast_hnfYGk1e_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_0pqCIpLd, HV_BINOP_EQ, 1, m, &cBinop_0pqCIpLd_sendMessage);
}

void Heavy_prog::cCast_bfMzhA1V_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_ecs7iZCZ_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_YpK0ifep_sendMessage);
}

void Heavy_prog::cBinop_ecs7iZCZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_J7MitxgM, 1, m, &cIf_J7MitxgM_sendMessage);
}

void Heavy_prog::cCast_5Y9i65KB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_GxdjkDXR_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_9oHijONQ_sendMessage);
}

void Heavy_prog::cCast_Hk6dcXL4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_J7MitxgM, 0, m, &cIf_J7MitxgM_sendMessage);
}

void Heavy_prog::cBinop_YpK0ifep_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_FwmYYSFq, 1, m, &cIf_FwmYYSFq_sendMessage);
}

void Heavy_prog::cBinop_0pqCIpLd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_uBKbDpL5, 1, m, &cIf_uBKbDpL5_sendMessage);
}

void Heavy_prog::cCast_GxdjkDXR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_D53w4PBO, 0, m, &cIf_D53w4PBO_sendMessage);
}

void Heavy_prog::cCast_9oHijONQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_y42Fxe5u, 1, m, &cVar_y42Fxe5u_sendMessage);
}

void Heavy_prog::cCast_PsDu1dS1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_HBWAAwww, 1, m, &cIf_HBWAAwww_sendMessage);
}

void Heavy_prog::cCast_CuT3M1Wr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_m01msqUy, 1, m, &cIf_m01msqUy_sendMessage);
}

void Heavy_prog::cCast_pBSMvPsu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_y42Fxe5u, 0, m, &cVar_y42Fxe5u_sendMessage);
}

void Heavy_prog::cCast_xE0v3whT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_D53w4PBO, 1, m, &cIf_D53w4PBO_sendMessage);
}

void Heavy_prog::cCast_t2jJhxCn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_J1sQKDP8_sendMessage);
}

void Heavy_prog::cCast_8HcwxaMl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_usePjgmC, 1, m, &cIf_usePjgmC_sendMessage);
}

void Heavy_prog::cCast_dQ00dvVK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_fJfAzJ58, 1, m, &cIf_fJfAzJ58_sendMessage);
}

void Heavy_prog::cVar_UN9BtfWM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_aWlG51OD_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_ij1jLCVz, 0, m, &cIf_ij1jLCVz_sendMessage);
}

void Heavy_prog::cUnop_wrfC2wDy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_S7M0L0Q2_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_mzCTo5Sa_sendMessage);
}

void Heavy_prog::cUnop_QpN30Liv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_S7M0L0Q2_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_mzCTo5Sa_sendMessage);
}

void Heavy_prog::cIf_ij1jLCVz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_QpN30Liv_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_wrfC2wDy_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_aWlG51OD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_ij1jLCVz, 1, m, &cIf_ij1jLCVz_sendMessage);
}

void Heavy_prog::cVar_YmwxP2GW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_LNusu3Xy_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_PdyPsZZA, 0, m, &cIf_PdyPsZZA_sendMessage);
}

void Heavy_prog::cUnop_bmKfE5Gu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_nqnscXy9_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5Th7olNo_sendMessage);
}

void Heavy_prog::cUnop_KdtjB2Wf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_nqnscXy9_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_5Th7olNo_sendMessage);
}

void Heavy_prog::cIf_PdyPsZZA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_KdtjB2Wf_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_bmKfE5Gu_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_LNusu3Xy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_PdyPsZZA, 1, m, &cIf_PdyPsZZA_sendMessage);
}

void Heavy_prog::cSlice_MQagWwIB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_cv1p0zSe_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_MEyI9vgm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_BbBN3Xdb, 0, m, &cIf_BbBN3Xdb_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_cdHhTG3r, HV_BINOP_EQ, 0, m, &cBinop_cdHhTG3r_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_Mz9IADzw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_cjosllpx, 0, m, &cIf_cjosllpx_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_2rGQlO9G, 0, m, &cIf_2rGQlO9G_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_cjosllpx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_hhRSrzIT, 0, m, &cVar_hhRSrzIT_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_BbBN3Xdb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_2rGQlO9G_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_XVcXxDhN, 0, m, &cIf_XVcXxDhN_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_tBmOUCOz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_jSsNankV, 0, m, &cIf_jSsNankV_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_XVcXxDhN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_bI2oXjHf, 0, m, &cIf_bI2oXjHf_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_bI2oXjHf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_hhRSrzIT, 0, m, &cVar_hhRSrzIT_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_jSsNankV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_sX4TYpdA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_QkuGfSQM, HV_BINOP_EQ, 0, m, &cBinop_QkuGfSQM_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_oS5japQ2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cVar_mT7drNaP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_tBmOUCOz, 0, m, &cIf_tBmOUCOz_sendMessage);
}

void Heavy_prog::cBinop_cv1p0zSe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_aRVWUveh_sendMessage);
}

void Heavy_prog::cBinop_aRVWUveh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_RIlDIKYU_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_n8PUkkoq_sendMessage);
}

void Heavy_prog::cBinop_cdHhTG3r_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xAsMO8s9_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_K541dnUz_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_96PuBxqC_sendMessage);
}

void Heavy_prog::cCast_mzCTo5Sa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_p8yAdM6C_sendMessage);
}

void Heavy_prog::cCast_S7M0L0Q2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_cdHhTG3r, HV_BINOP_EQ, 1, m, &cBinop_cdHhTG3r_sendMessage);
}

void Heavy_prog::cBinop_p8yAdM6C_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_JciWCE8H_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_C6iiMwgg_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xt6j2vZp_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_yDpG9Y1g_sendMessage);
}

void Heavy_prog::cBinop_UyqcH1Pw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_XVcXxDhN, 1, m, &cIf_XVcXxDhN_sendMessage);
}

void Heavy_prog::cCast_nqnscXy9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_fK26C1s0_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_AI77PzwL_sendMessage);
}

void Heavy_prog::cCast_5Th7olNo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_QkuGfSQM, HV_BINOP_EQ, 1, m, &cBinop_QkuGfSQM_sendMessage);
}

void Heavy_prog::cBinop_fK26C1s0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_sX4TYpdA, 1, m, &cIf_sX4TYpdA_sendMessage);
}

void Heavy_prog::cCast_n8PUkkoq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_Z9knXbGR_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_d1Dhyt8t_sendMessage);
}

void Heavy_prog::cCast_RIlDIKYU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_sX4TYpdA, 0, m, &cIf_sX4TYpdA_sendMessage);
}

void Heavy_prog::cBinop_AI77PzwL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_jSsNankV, 1, m, &cIf_jSsNankV_sendMessage);
}

void Heavy_prog::cBinop_QkuGfSQM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_bI2oXjHf, 1, m, &cIf_bI2oXjHf_sendMessage);
}

void Heavy_prog::cCast_d1Dhyt8t_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_mT7drNaP, 1, m, &cVar_mT7drNaP_sendMessage);
}

void Heavy_prog::cCast_Z9knXbGR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_oS5japQ2, 0, m, &cIf_oS5japQ2_sendMessage);
}

void Heavy_prog::cCast_96PuBxqC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_2rGQlO9G, 1, m, &cIf_2rGQlO9G_sendMessage);
}

void Heavy_prog::cCast_K541dnUz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_mT7drNaP, 0, m, &cVar_mT7drNaP_sendMessage);
}

void Heavy_prog::cCast_xAsMO8s9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_tBmOUCOz, 1, m, &cIf_tBmOUCOz_sendMessage);
}

void Heavy_prog::cCast_JciWCE8H_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_oS5japQ2, 1, m, &cIf_oS5japQ2_sendMessage);
}

void Heavy_prog::cCast_C6iiMwgg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_BbBN3Xdb, 1, m, &cIf_BbBN3Xdb_sendMessage);
}

void Heavy_prog::cCast_xt6j2vZp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_cjosllpx, 1, m, &cIf_cjosllpx_sendMessage);
}

void Heavy_prog::cCast_yDpG9Y1g_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_UyqcH1Pw_sendMessage);
}

void Heavy_prog::cVar_CU3FTxVb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_D7wwLQzh_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_TixPN7T5, 0, m, &cIf_TixPN7T5_sendMessage);
}

void Heavy_prog::cUnop_Co4pcFBW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_XLD3lN6y_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_PGb3Xwvx_sendMessage);
}

void Heavy_prog::cUnop_igM68kTg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_XLD3lN6y_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_PGb3Xwvx_sendMessage);
}

void Heavy_prog::cIf_TixPN7T5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_igM68kTg_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_Co4pcFBW_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_D7wwLQzh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_TixPN7T5, 1, m, &cIf_TixPN7T5_sendMessage);
}

void Heavy_prog::cVar_pqKqEp8f_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN, 0.0f, 0, m, &cBinop_4dWAoRxo_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_p2qq9c04, 0, m, &cIf_p2qq9c04_sendMessage);
}

void Heavy_prog::cUnop_i3b0lqJv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zfG6TXW9_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ZkHh8mCU_sendMessage);
}

void Heavy_prog::cUnop_I9dVUKPQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zfG6TXW9_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_ZkHh8mCU_sendMessage);
}

void Heavy_prog::cIf_p2qq9c04_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cUnop_onMessage(_c, HV_UNOP_CEIL, m, &cUnop_I9dVUKPQ_sendMessage);
      break;
    }
    case 1: {
      cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_i3b0lqJv_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_4dWAoRxo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_p2qq9c04, 1, m, &cIf_p2qq9c04_sendMessage);
}

void Heavy_prog::cSlice_Rw8chYSl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_kobBzGh5_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_rRoKs52Z_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_AP4W2tGn, 0, m, &cIf_AP4W2tGn_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_X9r4Ovjp, HV_BINOP_EQ, 0, m, &cBinop_X9r4Ovjp_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_eozYHLox_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cIf_onMessage(_c, &Context(_c)->cIf_G2HS1hDt, 0, m, &cIf_G2HS1hDt_sendMessage);
      cIf_onMessage(_c, &Context(_c)->cIf_SLkBtg8M, 0, m, &cIf_SLkBtg8M_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_G2HS1hDt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_KZ49bWkG, 0, m, &cVar_KZ49bWkG_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_AP4W2tGn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_SLkBtg8M_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_FWOe99S0, 0, m, &cIf_FWOe99S0_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_c7dtniuP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_mbt0fPu0, 0, m, &cIf_mbt0fPu0_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_FWOe99S0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cIf_onMessage(_c, &Context(_c)->cIf_ONQDH4pC, 0, m, &cIf_ONQDH4pC_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_ONQDH4pC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_KZ49bWkG, 0, m, &cVar_KZ49bWkG_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_mbt0fPu0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cIf_mvSxVdKn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_VdsbSiTw, HV_BINOP_EQ, 0, m, &cBinop_VdsbSiTw_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cIf_w3QcwT7n_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
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

void Heavy_prog::cVar_2uiQf6mR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_c7dtniuP, 0, m, &cIf_c7dtniuP_sendMessage);
}

void Heavy_prog::cBinop_kobBzGh5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_pxqcUNsM_sendMessage);
}

void Heavy_prog::cBinop_pxqcUNsM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_jCYRWMS0_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_6h171LmO_sendMessage);
}

void Heavy_prog::cBinop_X9r4Ovjp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_xQPNwJ5v_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_mjiNr8lY_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_QXV3JfBC_sendMessage);
}

void Heavy_prog::cCast_PGb3Xwvx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_CnSwQOiw_sendMessage);
}

void Heavy_prog::cCast_XLD3lN6y_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_X9r4Ovjp, HV_BINOP_EQ, 1, m, &cBinop_X9r4Ovjp_sendMessage);
}

void Heavy_prog::cBinop_CnSwQOiw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_8E6sKb0L_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_fVDFRWXb_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_X84aeyAR_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_bX8KzyIN_sendMessage);
}

void Heavy_prog::cBinop_rVkhX7H5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_FWOe99S0, 1, m, &cIf_FWOe99S0_sendMessage);
}

void Heavy_prog::cCast_zfG6TXW9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_NEQ, 0.0f, 0, m, &cBinop_ha5WvWzv_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_d6bbRP6c_sendMessage);
}

void Heavy_prog::cCast_ZkHh8mCU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_VdsbSiTw, HV_BINOP_EQ, 1, m, &cBinop_VdsbSiTw_sendMessage);
}

void Heavy_prog::cBinop_ha5WvWzv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_mvSxVdKn, 1, m, &cIf_mvSxVdKn_sendMessage);
}

void Heavy_prog::cCast_jCYRWMS0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_mvSxVdKn, 0, m, &cIf_mvSxVdKn_sendMessage);
}

void Heavy_prog::cCast_6h171LmO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_CjYpY9Vp_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_mSZ0HZLq_sendMessage);
}

void Heavy_prog::cBinop_d6bbRP6c_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_mbt0fPu0, 1, m, &cIf_mbt0fPu0_sendMessage);
}

void Heavy_prog::cBinop_VdsbSiTw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_ONQDH4pC, 1, m, &cIf_ONQDH4pC_sendMessage);
}

void Heavy_prog::cCast_mSZ0HZLq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_2uiQf6mR, 1, m, &cVar_2uiQf6mR_sendMessage);
}

void Heavy_prog::cCast_CjYpY9Vp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_w3QcwT7n, 0, m, &cIf_w3QcwT7n_sendMessage);
}

void Heavy_prog::cCast_QXV3JfBC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_SLkBtg8M, 1, m, &cIf_SLkBtg8M_sendMessage);
}

void Heavy_prog::cCast_xQPNwJ5v_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_c7dtniuP, 1, m, &cIf_c7dtniuP_sendMessage);
}

void Heavy_prog::cCast_mjiNr8lY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_2uiQf6mR, 0, m, &cVar_2uiQf6mR_sendMessage);
}

void Heavy_prog::cCast_bX8KzyIN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_rVkhX7H5_sendMessage);
}

void Heavy_prog::cCast_8E6sKb0L_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_w3QcwT7n, 1, m, &cIf_w3QcwT7n_sendMessage);
}

void Heavy_prog::cCast_X84aeyAR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_G2HS1hDt, 1, m, &cIf_G2HS1hDt_sendMessage);
}

void Heavy_prog::cCast_fVDFRWXb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_AP4W2tGn, 1, m, &cIf_AP4W2tGn_sendMessage);
}

void Heavy_prog::cSlice_5qn8a93m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_DrrQRHbZ_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_5y4CQ1SG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_XqpByOrX_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_aYWDHUoz_sendMessage);
      cSwitchcase_sFnTcmZl_onMessage(_c, NULL, 0, m, NULL);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_DrrQRHbZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cSystem_uB9y5VVc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_h346h3ME, HV_BINOP_SUBTRACT, 1, m, &cBinop_h346h3ME_sendMessage);
}

void Heavy_prog::cMsg_NE9KtYMs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "currentTime");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_uB9y5VVc_sendMessage);
}

void Heavy_prog::cBinop_h346h3ME_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_WgEQlcV8, HV_BINOP_DIVIDE, 0, m, &cBinop_WgEQlcV8_sendMessage);
}

void Heavy_prog::cSystem_JPFfiBNs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_h346h3ME, HV_BINOP_SUBTRACT, 0, m, &cBinop_h346h3ME_sendMessage);
}

void Heavy_prog::cMsg_Cd2D1eL1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "currentTime");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_JPFfiBNs_sendMessage);
}

void Heavy_prog::cBinop_WgEQlcV8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_cvD4pqEN_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cSystem_K93MUqZA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_8ROvlF9E_sendMessage);
}

void Heavy_prog::cMsg_3VS3nq1G_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_K93MUqZA_sendMessage);
}

void Heavy_prog::cBinop_8ROvlF9E_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_WgEQlcV8, HV_BINOP_DIVIDE, 1, m, &cBinop_WgEQlcV8_sendMessage);
}

void Heavy_prog::cSwitchcase_cvD4pqEN_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Hqs3y7Tj_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_Hqs3y7Tj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_5fVQ4AMs_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_KH4fAj73_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_tlWDCSL5_sendMessage);
}

void Heavy_prog::cIf_hemXwpKW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cVar_onMessage(_c, &Context(_c)->cVar_HmcZEssK, 0, m, &cVar_HmcZEssK_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_FmsqelSG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_hemXwpKW, 1, m, &cIf_hemXwpKW_sendMessage);
}

void Heavy_prog::cVar_HmcZEssK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_GuCzNiGX_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_V4bTJaJr, 0, m, &cVar_V4bTJaJr_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_GREATER_THAN_EQL, 96.0f, 0, m, &cBinop_niX2xuUS_sendMessage);
  cIf_onMessage(_c, &Context(_c)->cIf_Ud8Q5CmV, 0, m, &cIf_Ud8Q5CmV_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MOD_UNIPOLAR, 4.0f, 0, m, &cBinop_unJU57QK_sendMessage);
}

void Heavy_prog::cVar_nUtyzNav_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_bCSQTkBs_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_MJgn6QqM_sendMessage);
}

void Heavy_prog::cSwitchcase_sFnTcmZl_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x437A0000: { // "250.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_KZdiiaBs_sendMessage);
      break;
    }
    case 0x437C0000: { // "252.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_7nBh8vy8_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_KZdiiaBs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_0vS00PCf_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_7nBh8vy8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_ducJXvB8_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cSwitchcase_ducJXvB8_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cMsg_Qewso1G8_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cMsg_Qewso1G8_sendMessage(_c, 0, m);
      cDelay_onMessage(_c, &Context(_c)->cDelay_WFAClOKM, 1, m, &cDelay_WFAClOKM_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_orULR668_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_WFAClOKM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_WFAClOKM, m);
  cMsg_v3o1ZYrG_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_Qewso1G8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_WFAClOKM, 0, m, &cDelay_WFAClOKM_sendMessage);
}

void Heavy_prog::cCast_orULR668_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_WFAClOKM, 0, m, &cDelay_WFAClOKM_sendMessage);
}

void Heavy_prog::cIf_Ud8Q5CmV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_XMlKgdD8_sendMessage);
      break;
    }
    case 1: {
      cMsg_QTYAzM7D_sendMessage(_c, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_niX2xuUS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_Ud8Q5CmV, 1, m, &cIf_Ud8Q5CmV_sendMessage);
}

void Heavy_prog::cVar_1Oa3asUj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_HmcZEssK, 1, m, &cVar_HmcZEssK_sendMessage);
}

void Heavy_prog::cSwitchcase_i14GQwuQ_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_bLAZxrNG_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_bLAZxrNG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_4sL4NBXT, 0, m, &cVar_4sL4NBXT_sendMessage);
}

void Heavy_prog::cVar_wKBe7Ovk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_w43sFxf7_sendMessage);
  cSwitchcase_W2Vrvu2w_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cVar_4sL4NBXT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_DN0z2suc_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MOD_UNIPOLAR, 16.0f, 0, m, &cBinop_XIQvXAHp_sendMessage);
}

void Heavy_prog::cSwitchcase_2uiZ6JHc_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cMsg_sQyU6sXZ_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cMsg_sQyU6sXZ_sendMessage(_c, 0, m);
      cDelay_onMessage(_c, &Context(_c)->cDelay_kqtLrc4a, 1, m, &cDelay_kqtLrc4a_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_1Cq9TXnt_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_kqtLrc4a_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_kqtLrc4a, m);
  cVar_onMessage(_c, &Context(_c)->cVar_FmsqelSG, 0, m, &cVar_FmsqelSG_sendMessage);
}

void Heavy_prog::cMsg_sQyU6sXZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_kqtLrc4a, 0, m, &cDelay_kqtLrc4a_sendMessage);
}

void Heavy_prog::cCast_1Cq9TXnt_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_kqtLrc4a, 0, m, &cDelay_kqtLrc4a_sendMessage);
}

void Heavy_prog::cCast_XqpByOrX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_3VS3nq1G_sendMessage(_c, 0, m);
  cMsg_Cd2D1eL1_sendMessage(_c, 0, m);
  cIf_onMessage(_c, &Context(_c)->cIf_hemXwpKW, 0, m, &cIf_hemXwpKW_sendMessage);
}

void Heavy_prog::cCast_aYWDHUoz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_NE9KtYMs_sendMessage(_c, 0, m);
}

void Heavy_prog::cMsg_QTYAzM7D_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_HmcZEssK, 1, m, &cVar_HmcZEssK_sendMessage);
}

void Heavy_prog::cBinop_XMlKgdD8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_HmcZEssK, 1, m, &cVar_HmcZEssK_sendMessage);
}

void Heavy_prog::cCast_tlWDCSL5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_1Oa3asUj, 0, m, &cVar_1Oa3asUj_sendMessage);
}

void Heavy_prog::cCast_5fVQ4AMs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_nUtyzNav, 0, m, &cVar_nUtyzNav_sendMessage);
}

void Heavy_prog::cCast_KH4fAj73_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_2uiZ6JHc_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cCast_MJgn6QqM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_HmcZEssK, 0, m, &cVar_HmcZEssK_sendMessage);
}

void Heavy_prog::cCast_bCSQTkBs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cIf_onMessage(_c, &Context(_c)->cIf_hemXwpKW, 1, m, &cIf_hemXwpKW_sendMessage);
}

void Heavy_prog::cBinop_TFGGs90p_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_kqtLrc4a, 1, m, &cDelay_kqtLrc4a_sendMessage);
}

void Heavy_prog::cMsg_0vS00PCf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cPrint_onMessage(_c, m, "start");
  cSwitchcase_cvD4pqEN_onMessage(_c, NULL, 0, m, NULL);
  cVar_onMessage(_c, &Context(_c)->cVar_wKBe7Ovk, 0, m, &cVar_wKBe7Ovk_sendMessage);
}

void Heavy_prog::cMsg_v3o1ZYrG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cPrint_onMessage(_c, m, "stop");
  cIf_onMessage(_c, &Context(_c)->cIf_hemXwpKW, 1, m, &cIf_hemXwpKW_sendMessage);
}

void Heavy_prog::cMsg_KpeCIZTC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 60000.0f);
  msg_setElementToFrom(m, 1, n, 0);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 0.0f, 0, m, &cBinop_TFGGs90p_sendMessage);
}

void Heavy_prog::cBinop_unJU57QK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_awzPqM2i_sendMessage);
}

void Heavy_prog::cBinop_awzPqM2i_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_i14GQwuQ_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cBinop_DN0z2suc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_4sL4NBXT, 1, m, &cVar_4sL4NBXT_sendMessage);
}

void Heavy_prog::cBinop_XIQvXAHp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_wKBe7Ovk, 0, m, &cVar_wKBe7Ovk_sendMessage);
}

void Heavy_prog::cVar_RP75dAoa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_KpeCIZTC_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_GuCzNiGX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cCast_w43sFxf7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cVar_V4bTJaJr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cMsg_ZOQkEYNH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_q4g1KXl7_sendMessage);
}

void Heavy_prog::cSystem_q4g1KXl7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_RRRzi83x, HV_BINOP_MULTIPLY, 1, m, &cBinop_RRRzi83x_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_PdZuTyNT, HV_BINOP_MULTIPLY, 1, m, &cBinop_PdZuTyNT_sendMessage);
}

void Heavy_prog::cSwitchcase_5xXU3ejn_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cMsg_QpKUZPis_sendMessage(_c, 0, m);
      break;
    }
    case 0x7A5B032D: { // "stop"
      cMsg_QpKUZPis_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_AoXNzBls_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cDelay_VX2x1n2f_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_VX2x1n2f, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_VX2x1n2f, 0, m, &cDelay_VX2x1n2f_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_PAEnssjw, 0, m, &cVar_PAEnssjw_sendMessage);
}

void Heavy_prog::cCast_AoXNzBls_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_QpKUZPis_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_VX2x1n2f, 0, m, &cDelay_VX2x1n2f_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_PAEnssjw, 0, m, &cVar_PAEnssjw_sendMessage);
}

void Heavy_prog::cMsg_Bf7Nysqp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_AK3tjBEF_sendMessage);
}

void Heavy_prog::cSystem_AK3tjBEF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_7DhszMrJ_sendMessage);
}

void Heavy_prog::cVar_sCwPzZi3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_vIkdJ3MH, HV_BINOP_MULTIPLY, 0, m, &cBinop_vIkdJ3MH_sendMessage);
}

void Heavy_prog::cMsg_QpKUZPis_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_VX2x1n2f, 0, m, &cDelay_VX2x1n2f_sendMessage);
}

void Heavy_prog::cBinop_M0zOkD20_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_VX2x1n2f, 2, m, &cDelay_VX2x1n2f_sendMessage);
}

void Heavy_prog::cBinop_7DhszMrJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_vIkdJ3MH, HV_BINOP_MULTIPLY, 1, m, &cBinop_vIkdJ3MH_sendMessage);
}

void Heavy_prog::cBinop_vIkdJ3MH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_M0zOkD20_sendMessage);
}

void Heavy_prog::cVar_PAEnssjw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_JmRjXbmc, HV_BINOP_SUBTRACT, 0, m, &cBinop_JmRjXbmc_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_LESS_THAN_EQL, 0.0f, 0, m, &cBinop_tkMCzh4q_sendMessage);
}

void Heavy_prog::cSwitchcase_Ee8ssv5I_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_T4NvQIrk_sendMessage);
      break;
    }
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Ex6u9DpX_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cCast_T4NvQIrk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_bgSaBrGb, 0, m, &cVar_bgSaBrGb_sendMessage);
}

void Heavy_prog::cCast_Ex6u9DpX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_EI1xiieI_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_lbG5EA58_sendMessage);
}

void Heavy_prog::cSwitchcase_BNN31wF3_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cSlice_onMessage(_c, &Context(_c)->cSlice_sIFKTEjy, 0, m, &cSlice_sIFKTEjy_sendMessage);
      break;
    }
    case 0x3E004DAB: { // "set"
      cSlice_onMessage(_c, &Context(_c)->cSlice_WVO8No4n, 0, m, &cSlice_WVO8No4n_sendMessage);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_0fWSOvqi_sendMessage);
      cSlice_onMessage(_c, &Context(_c)->cSlice_pPsGdLJ9, 0, m, &cSlice_pPsGdLJ9_sendMessage);
      cSlice_onMessage(_c, &Context(_c)->cSlice_4L6qFKQr, 0, m, &cSlice_4L6qFKQr_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_qNbqNt3Q_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_6GtRuvfB_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cSlice_sIFKTEjy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cMsg_htxliDdx_sendMessage(_c, 0, m);
      break;
    }
    case 1: {
      cMsg_htxliDdx_sendMessage(_c, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_WVO8No4n_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_ziIzkQhH_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_QGCrFTjz_sendMessage);
      break;
    }
    case 1: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_ziIzkQhH_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_QGCrFTjz_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cVar_5HnEabZn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_I3vJ9WKk_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_HP7TgnGm_sendMessage);
}

void Heavy_prog::cVar_E2pYkKTr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_Xnr7yvbR_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cSwitchcase_Xnr7yvbR_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_HjNmV8Ly_sendMessage);
      break;
    }
    default: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_RRRzi83x, HV_BINOP_MULTIPLY, 0, m, &cBinop_RRRzi83x_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_a00iGGhj, HV_BINOP_DIVIDE, 1, m, &cBinop_a00iGGhj_sendMessage);
      cVar_onMessage(_c, &Context(_c)->cVar_sCwPzZi3, 0, m, &cVar_sCwPzZi3_sendMessage);
      break;
    }
  }
}

void Heavy_prog::cCast_HjNmV8Ly_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_LmI0DSyC_sendMessage(_c, 0, m);
}

void Heavy_prog::cVar_0JxmXT6k_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_uNP7rWZx, HV_BINOP_SUBTRACT, 1, m, &cBinop_uNP7rWZx_sendMessage);
}

void Heavy_prog::cVar_RgJv1kIW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_bgSaBrGb, 0, m, &cVar_bgSaBrGb_sendMessage);
}

void Heavy_prog::cVar_bgSaBrGb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_MgNdjdBF, HV_BINOP_ADD, 0, m, &cBinop_MgNdjdBF_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_arv6taHI, HV_BINOP_ADD, 0, m, &cBinop_arv6taHI_sendMessage);
  sVarf_onMessage(_c, &Context(_c)->sVarf_uGfFQ7KK, m);
  sVarf_onMessage(_c, &Context(_c)->sVarf_HBUbZLMB, m);
}

void Heavy_prog::cSlice_pPsGdLJ9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_I3vJ9WKk_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_HP7TgnGm_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cSlice_4L6qFKQr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_6ayzkQs0_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_zwAaPBX7_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_LuFBdPUg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_PAEnssjw, 1, m, &cVar_PAEnssjw_sendMessage);
}

void Heavy_prog::cBinop_PdZuTyNT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_LuFBdPUg_sendMessage);
}

void Heavy_prog::cBinop_RRRzi83x_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_mzGeDqRn_sendMessage);
}

void Heavy_prog::cBinop_mzGeDqRn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_JmRjXbmc, HV_BINOP_SUBTRACT, 1, m, &cBinop_JmRjXbmc_sendMessage);
}

void Heavy_prog::cBinop_JmRjXbmc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_PAEnssjw, 1, m, &cVar_PAEnssjw_sendMessage);
}

void Heavy_prog::cMsg_ws7lyiDP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cSwitchcase_5xXU3ejn_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_fGlue7LF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cSwitchcase_5xXU3ejn_onMessage(_c, NULL, 0, m, NULL);
  cBinop_onMessage(_c, &Context(_c)->cBinop_arv6taHI, HV_BINOP_ADD, 1, m, &cBinop_arv6taHI_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_MgNdjdBF, HV_BINOP_ADD, 1, m, &cBinop_MgNdjdBF_sendMessage);
}

void Heavy_prog::cBinop_tkMCzh4q_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_Ee8ssv5I_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cBinop_MgNdjdBF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_bgSaBrGb, 1, m, &cVar_bgSaBrGb_sendMessage);
}

void Heavy_prog::cBinop_a00iGGhj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_QDZYi7ql, HV_BINOP_DIVIDE, 1, m, &cBinop_QDZYi7ql_sendMessage);
}

void Heavy_prog::cBinop_QDZYi7ql_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_arv6taHI, HV_BINOP_ADD, 1, m, &cBinop_arv6taHI_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_MgNdjdBF, HV_BINOP_ADD, 1, m, &cBinop_MgNdjdBF_sendMessage);
}

void Heavy_prog::cCast_HP7TgnGm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_a00iGGhj, HV_BINOP_DIVIDE, 0, m, &cBinop_a00iGGhj_sendMessage);
}

void Heavy_prog::cCast_I3vJ9WKk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_PdZuTyNT, HV_BINOP_MULTIPLY, 0, m, &cBinop_PdZuTyNT_sendMessage);
}

void Heavy_prog::cCast_6ayzkQs0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_RgJv1kIW, 1, m, &cVar_RgJv1kIW_sendMessage);
}

void Heavy_prog::cCast_zwAaPBX7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_uNP7rWZx, HV_BINOP_SUBTRACT, 0, m, &cBinop_uNP7rWZx_sendMessage);
}

void Heavy_prog::cCast_lbG5EA58_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_RgJv1kIW, 0, m, &cVar_RgJv1kIW_sendMessage);
}

void Heavy_prog::cCast_EI1xiieI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_fGlue7LF_sendMessage(_c, 0, m);
}

void Heavy_prog::cBinop_arv6taHI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_0JxmXT6k, 0, m, &cVar_0JxmXT6k_sendMessage);
}

void Heavy_prog::cMsg_htxliDdx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cSwitchcase_5xXU3ejn_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cMsg_xDm6XZwN_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_5HnEabZn, 1, m, &cVar_5HnEabZn_sendMessage);
}

void Heavy_prog::cMsg_LmI0DSyC_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 20.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_RRRzi83x, HV_BINOP_MULTIPLY, 0, m, &cBinop_RRRzi83x_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_a00iGGhj, HV_BINOP_DIVIDE, 1, m, &cBinop_a00iGGhj_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_sCwPzZi3, 0, m, &cVar_sCwPzZi3_sendMessage);
}

void Heavy_prog::cCast_ziIzkQhH_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_htxliDdx_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_QGCrFTjz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_001fHcPc_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_arv6taHI, HV_BINOP_ADD, 0, m, &cBinop_arv6taHI_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_bgSaBrGb, 1, m, &cVar_bgSaBrGb_sendMessage);
}

void Heavy_prog::cBinop_uNP7rWZx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_QDZYi7ql, HV_BINOP_DIVIDE, 0, m, &cBinop_QDZYi7ql_sendMessage);
}

void Heavy_prog::cCast_001fHcPc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_fGlue7LF_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_0fWSOvqi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_5HnEabZn, 0, m, &cVar_5HnEabZn_sendMessage);
}

void Heavy_prog::cCast_qNbqNt3Q_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_ws7lyiDP_sendMessage(_c, 0, m);
}

void Heavy_prog::cCast_6GtRuvfB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_xDm6XZwN_sendMessage(_c, 0, m);
}

void Heavy_prog::cSwitchcase_W2Vrvu2w_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x41000000: { // "8.0"
      cSlice_onMessage(_c, &Context(_c)->cSlice_DfcbhiHe, 0, m, &cSlice_DfcbhiHe_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_prog::cSlice_DfcbhiHe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cMsg_PARtkqGW_sendMessage(_c, 0, m);
      break;
    }
    case 1: {
      cMsg_PARtkqGW_sendMessage(_c, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_prog::cBinop_53kk02Ay_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
}

void Heavy_prog::cBinop_sp3QIAbc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 4.0f, 0, m, &cBinop_MYEPFcRK_sendMessage);
}

void Heavy_prog::cCast_ZdVr1TNR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Bz7qHPTi, HV_BINOP_SUBTRACT, 0, m, &cBinop_Bz7qHPTi_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_HjgLSHyf, HV_BINOP_ADD, 0, m, &cBinop_HjgLSHyf_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_ryTE9SQW, 0, m, &cVar_ryTE9SQW_sendMessage);
}

void Heavy_prog::cCast_QsLnZIRp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Bz7qHPTi, HV_BINOP_SUBTRACT, 1, m, &cBinop_Bz7qHPTi_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_HjgLSHyf, HV_BINOP_ADD, 1, m, &cBinop_HjgLSHyf_sendMessage);
}

void Heavy_prog::cMsg_R7dpVxsU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 80.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_ryTE9SQW, 0, m, &cVar_ryTE9SQW_sendMessage);
}

void Heavy_prog::cBinop_4N6SktEw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_nE6V0vNq, 0, m, &cVar_nE6V0vNq_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_S1LcWebf, 0, m, &cVar_S1LcWebf_sendMessage);
}

void Heavy_prog::cBinop_Bz7qHPTi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_u3GW6FJi, m);
}

void Heavy_prog::cBinop_HjgLSHyf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sVarf_onMessage(_c, &Context(_c)->sVarf_r4MmzX2O, m);
}

void Heavy_prog::cBinop_MYEPFcRK_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_QsLnZIRp_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_ZdVr1TNR_sendMessage);
}

void Heavy_prog::cCast_R3zudfjE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_HjgLSHyf, HV_BINOP_ADD, 0, m, &cBinop_HjgLSHyf_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_Bz7qHPTi, HV_BINOP_SUBTRACT, 0, m, &cBinop_Bz7qHPTi_sendMessage);
}

void Heavy_prog::cCast_0W6AYdU0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Bz7qHPTi, HV_BINOP_SUBTRACT, 0, m, &cBinop_Bz7qHPTi_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_HjgLSHyf, HV_BINOP_ADD, 0, m, &cBinop_HjgLSHyf_sendMessage);
}

void Heavy_prog::cMsg_LnHfxKLW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 120.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_RP75dAoa, 0, m, &cVar_RP75dAoa_sendMessage);
}

void Heavy_prog::cMsg_PARtkqGW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cSwitchcase_BNN31wF3_onMessage(_c, NULL, 0, m, NULL);
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 200.0f);
  cSwitchcase_BNN31wF3_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_prog::cReceive_3uCiN09j_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_PJJ1322p_sendMessage(_c, 0, m);
  cMsg_FPWg0ceu_sendMessage(_c, 0, m);
  cMsg_HIB95DBA_sendMessage(_c, 0, m);
  cMsg_UDmcWmgT_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_nE6V0vNq, 0, m, &cVar_nE6V0vNq_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_S1LcWebf, 0, m, &cVar_S1LcWebf_sendMessage);
  cMsg_Bf7Nysqp_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_sCwPzZi3, 0, m, &cVar_sCwPzZi3_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_L2Z4fpc9, 0, m, &cVar_L2Z4fpc9_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_PdHssghP, 0, m, &cVar_PdHssghP_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_kBYtz8Om, 0, m, &cVar_kBYtz8Om_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_YmwxP2GW, 0, m, &cVar_YmwxP2GW_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_UN9BtfWM, 0, m, &cVar_UN9BtfWM_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_pqKqEp8f, 0, m, &cVar_pqKqEp8f_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_CU3FTxVb, 0, m, &cVar_CU3FTxVb_sendMessage);
  cMsg_ZOQkEYNH_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_0JxmXT6k, 0, m, &cVar_0JxmXT6k_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_E2pYkKTr, 0, m, &cVar_E2pYkKTr_sendMessage);
  cMsg_R7dpVxsU_sendMessage(_c, 0, m);
  cMsg_LnHfxKLW_sendMessage(_c, 0, m);
}

void Heavy_prog::cReceive_Qztg4E12_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_csEYG6Y8, HV_BINOP_MULTIPLY, 0, m, &cBinop_csEYG6Y8_sendMessage);
}

void Heavy_prog::cReceive_sZeJnywv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_4kWurzSp_sendMessage(_c, 0, m);
  cMsg_LrArsgSJ_sendMessage(_c, 0, m);
  cMsg_yVce42d4_sendMessage(_c, 0, m);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -2.0f, 0, m, &cBinop_G9icV9sA_sendMessage);
}

void Heavy_prog::cReceive_96myYRUQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_dKEfjQWo_sendMessage);
  cMsg_7Oe4Vvc2_sendMessage(_c, 0, m);
}

void Heavy_prog::cReceive_2xUMxCfG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_6l6tsWEy, HV_BINOP_MULTIPLY, 0, m, &cBinop_6l6tsWEy_sendMessage);
}

void Heavy_prog::cReceive_9Wj8ua1u_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_vkDwq3MS_sendMessage(_c, 0, m);
  cMsg_POgAUptY_sendMessage(_c, 0, m);
  cMsg_hE3Lv4Zj_sendMessage(_c, 0, m);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -2.0f, 0, m, &cBinop_SFfuAuyv_sendMessage);
}

void Heavy_prog::cReceive_VH4sTuxJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 1.0f, 0, m, &cBinop_QUtbNYXe_sendMessage);
  cMsg_zVy88w3J_sendMessage(_c, 0, m);
}

void Heavy_prog::cReceive_Z5DvwMdG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_KZ49bWkG, 0, m, &cVar_KZ49bWkG_sendMessage);
}

void Heavy_prog::cReceive_TW0sjGKd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_ryTE9SQW, 0, m, &cVar_ryTE9SQW_sendMessage);
}

void Heavy_prog::cReceive_pdHj3YN1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_hhRSrzIT, 0, m, &cVar_hhRSrzIT_sendMessage);
}

void Heavy_prog::cReceive_BbyYEiN5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_56I05L6n, 0, m, &cSlice_56I05L6n_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_fJKLxr87, 0, m, &cSlice_fJKLxr87_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_UcBtKPmQ, 0, m, &cSlice_UcBtKPmQ_sendMessage);
}

void Heavy_prog::cReceive_76ajcvoG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_ML9hroip, 0, m, &cSlice_ML9hroip_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_ToVtZBP2, 0, m, &cSlice_ToVtZBP2_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_uDrgw2R4, 0, m, &cSlice_uDrgw2R4_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_MQagWwIB, 0, m, &cSlice_MQagWwIB_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_MEyI9vgm, 0, m, &cSlice_MEyI9vgm_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_Mz9IADzw, 0, m, &cSlice_Mz9IADzw_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_Rw8chYSl, 0, m, &cSlice_Rw8chYSl_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_rRoKs52Z, 0, m, &cSlice_rRoKs52Z_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_eozYHLox, 0, m, &cSlice_eozYHLox_sendMessage);
}

void Heavy_prog::cReceive_nIi8cd76_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSlice_onMessage(_c, &Context(_c)->cSlice_5qn8a93m, 0, m, &cSlice_5qn8a93m_sendMessage);
  cSlice_onMessage(_c, &Context(_c)->cSlice_5y4CQ1SG, 0, m, &cSlice_5y4CQ1SG_sendMessage);
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
    __hv_varread_f(&sVarf_r4MmzX2O, VOf(Bf0));
    __hv_phasor_f(&sPhasor_yQsb2nIb, VIf(Bf0), VOf(Bf1));
    __hv_add_f(VIf(Bf1), VIf(Bf1), VOf(Bf2));
    __hv_var_k_f(VOf(Bf3), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_sub_f(VIf(Bf2), VIf(Bf3), VOf(Bf3));
    __hv_varread_f(&sVarf_aJl09ddZ, VOf(Bf2));
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
    __hv_line_f(&sLine_wO3u3tui, VOf(Bf3));
    __hv_line_f(&sLine_Jr4j5thQ, VOf(Bf7));
    __hv_line_f(&sLine_KxpjAxuF, VOf(Bf4));
    __hv_line_f(&sLine_dR45bG2s, VOf(Bf1));
    __hv_line_f(&sLine_YsEKG2MS, VOf(Bf0));
    __hv_biquad_f(&sBiquad_s_egl1Ng4C, VIf(Bf2), VIf(Bf3), VIf(Bf7), VIf(Bf4), VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_varread_f(&sVarf_HBUbZLMB, VOf(Bf1));
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf0), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O1), VOf(O1));
    __hv_varread_f(&sVarf_u3GW6FJi, VOf(Bf0));
    __hv_phasor_f(&sPhasor_m2VEJZnO, VIf(Bf0), VOf(Bf1));
    __hv_add_f(VIf(Bf1), VIf(Bf1), VOf(Bf4));
    __hv_var_k_f(VOf(Bf7), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    __hv_sub_f(VIf(Bf4), VIf(Bf7), VOf(Bf7));
    __hv_varread_f(&sVarf_e0srBG24, VOf(Bf4));
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
    __hv_line_f(&sLine_h6bPVxoO, VOf(Bf7));
    __hv_line_f(&sLine_2kOE3uyS, VOf(Bf6));
    __hv_line_f(&sLine_NdyfbPvE, VOf(Bf3));
    __hv_line_f(&sLine_mkvCZZvv, VOf(Bf1));
    __hv_line_f(&sLine_26t4Armx, VOf(Bf0));
    __hv_biquad_f(&sBiquad_s_uPnadhNU, VIf(Bf4), VIf(Bf7), VIf(Bf6), VIf(Bf3), VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_varread_f(&sVarf_uGfFQ7KK, VOf(Bf1));
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
