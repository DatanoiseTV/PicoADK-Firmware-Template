// PSRAM HAL — RP2350.
//
// Direct-mode QMI bring-up + TLSF heap. The detect + init routines below are
// derived from MicroPython's rp2_psram.c and Arduino-Pico's psram.cpp (both
// MIT-licensed); the picoadk integration (TLSF hook-up, clock-change retune,
// linker sections) is our own.
//
// -----------------------------------------------------------------------------
// License notice (for the detect + init routines)
// -----------------------------------------------------------------------------
// The MIT License (MIT)
// Copyright (c) MicroPython project contributors
// Copyright (c) 2024 SparkFun Electronics
// Copyright (c) 2025 Phil Howard, Mike Bell, Kirk D. Benell (Arduino-Pico)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions: the
// above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

#include "picoadk/hal/psram.h"
#include "picoadk/hal/system.h"
#include "picoadk_board.h"
#include "tlsf.h"

#include "pico/stdlib.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#if PICOADK_BOARD_V2

#include "hardware/address_mapped.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/regs/addressmap.h"
#include "hardware/sync.h"
#include "hardware/structs/ioqspi.h"
#include "hardware/structs/qmi.h"
#include "hardware/structs/xip_ctrl.h"

#ifndef PICOADK_PSRAM_CS_PIN
#  define PICOADK_PSRAM_CS_PIN 0
#endif
#ifndef PICOADK_PSRAM_MAX_SCK_HZ
#  define PICOADK_PSRAM_MAX_SCK_HZ 109'000'000u
#endif

extern "C" {
extern uint8_t __psram_dynamic_start[] __attribute__((weak));
extern uint8_t __psram_dynamic_end  [] __attribute__((weak));
}

namespace {

// Detect: read APS6404 / ISSI JEDEC ID via QMI direct mode, return bytes.
std::size_t __no_inline_not_in_flash_func(psram_detect)() {
    std::size_t size = 0;

    qmi_hw->direct_csr = 30u << QMI_DIRECT_CSR_CLKDIV_LSB | QMI_DIRECT_CSR_EN_BITS;
    while ((qmi_hw->direct_csr & QMI_DIRECT_CSR_BUSY_BITS) != 0) {}
    qmi_hw->direct_csr |= QMI_DIRECT_CSR_ASSERT_CS1N_BITS;

    qmi_hw->direct_tx = QMI_DIRECT_TX_OE_BITS |
                        (QMI_DIRECT_TX_IWIDTH_VALUE_Q << QMI_DIRECT_TX_IWIDTH_LSB) |
                        0xf5u;
    while ((qmi_hw->direct_csr & QMI_DIRECT_CSR_BUSY_BITS) != 0) {}
    (void)qmi_hw->direct_rx;
    qmi_hw->direct_csr &= ~(QMI_DIRECT_CSR_ASSERT_CS1N_BITS);

    qmi_hw->direct_csr |= QMI_DIRECT_CSR_ASSERT_CS1N_BITS;
    uint8_t kgd = 0, eid = 0;
    for (int i = 0; i < 7; ++i) {
        qmi_hw->direct_tx = (i == 0) ? 0x9fu : 0xffu;
        while ((qmi_hw->direct_csr & QMI_DIRECT_CSR_TXEMPTY_BITS) == 0) {}
        while ((qmi_hw->direct_csr & QMI_DIRECT_CSR_BUSY_BITS)    != 0) {}
        if      (i == 5) kgd = (uint8_t)qmi_hw->direct_rx;
        else if (i == 6) eid = (uint8_t)qmi_hw->direct_rx;
        else             (void)qmi_hw->direct_rx;
    }
    qmi_hw->direct_csr &= ~(QMI_DIRECT_CSR_ASSERT_CS1N_BITS | QMI_DIRECT_CSR_EN_BITS);

    if (kgd == 0x5D) {
        size = 1024u * 1024u;
        uint8_t size_id = eid >> 5;
        if      (size_id == 4)                                    size *= 16;
        else if (eid == 0x26 || size_id == 2 || size_id == 3)     size *= 8;
        else if (size_id == 0)                                    size *= 2;
        else if (size_id == 1)                                    size *= 4;
    }
    return size;
}

// Bring up QMI m[1] for the detected chip. Safe to call repeatedly (used on
// clock change).
std::size_t __no_inline_not_in_flash_func(psram_init_hw)(unsigned cs_pin) {
    gpio_set_function(cs_pin, GPIO_FUNC_XIP_CS1);
    uint32_t saved = save_and_disable_interrupts();

    std::size_t size = psram_detect();
    if (!size) { restore_interrupts(saved); return 0; }

    qmi_hw->direct_csr = 10u << QMI_DIRECT_CSR_CLKDIV_LSB |
                         QMI_DIRECT_CSR_EN_BITS |
                         QMI_DIRECT_CSR_AUTO_CS1N_BITS;
    while (qmi_hw->direct_csr & QMI_DIRECT_CSR_BUSY_BITS) {}

    qmi_hw->direct_tx = QMI_DIRECT_TX_NOPUSH_BITS | 0x35u;   // enable QPI
    while (qmi_hw->direct_csr & QMI_DIRECT_CSR_BUSY_BITS) {}

    const int clock_hz = clock_get_hz(clk_sys);
    int divisor = (clock_hz + (int)PICOADK_PSRAM_MAX_SCK_HZ - 1) / (int)PICOADK_PSRAM_MAX_SCK_HZ;
    if (divisor == 1 && clock_hz > 100'000'000) divisor = 2;
    int rxdelay = divisor;
    if (clock_hz / divisor > 100'000'000) rxdelay += 1;

    const long long period_fs = 1'000'000'000'000'000ll / clock_hz;
    const int max_select      = (int)((125 * 1'000'000ll) / period_fs);
    const int min_deselect    = (int)((18 * 1'000'000ll + (period_fs - 1)) / period_fs)
                                - (divisor + 1) / 2;

    qmi_hw->m[1].timing = 1u << QMI_M1_TIMING_COOLDOWN_LSB |
                          (QMI_M1_TIMING_PAGEBREAK_VALUE_1024 << QMI_M1_TIMING_PAGEBREAK_LSB) |
                          ((uint32_t)max_select  << QMI_M1_TIMING_MAX_SELECT_LSB) |
                          ((uint32_t)min_deselect<< QMI_M1_TIMING_MIN_DESELECT_LSB) |
                          ((uint32_t)rxdelay     << QMI_M1_TIMING_RXDELAY_LSB) |
                          ((uint32_t)divisor     << QMI_M1_TIMING_CLKDIV_LSB);

    qmi_hw->m[1].rfmt =
        (QMI_M0_RFMT_PREFIX_WIDTH_VALUE_Q << QMI_M0_RFMT_PREFIX_WIDTH_LSB) |
        (QMI_M0_RFMT_ADDR_WIDTH_VALUE_Q   << QMI_M0_RFMT_ADDR_WIDTH_LSB)   |
        (QMI_M0_RFMT_SUFFIX_WIDTH_VALUE_Q << QMI_M0_RFMT_SUFFIX_WIDTH_LSB) |
        (QMI_M0_RFMT_DUMMY_WIDTH_VALUE_Q  << QMI_M0_RFMT_DUMMY_WIDTH_LSB)  |
        (QMI_M0_RFMT_DATA_WIDTH_VALUE_Q   << QMI_M0_RFMT_DATA_WIDTH_LSB)   |
        (QMI_M0_RFMT_PREFIX_LEN_VALUE_8   << QMI_M0_RFMT_PREFIX_LEN_LSB)   |
        (6u << QMI_M0_RFMT_DUMMY_LEN_LSB);
    qmi_hw->m[1].rcmd = 0xEB;

    qmi_hw->m[1].wfmt =
        (QMI_M0_WFMT_PREFIX_WIDTH_VALUE_Q << QMI_M0_WFMT_PREFIX_WIDTH_LSB) |
        (QMI_M0_WFMT_ADDR_WIDTH_VALUE_Q   << QMI_M0_WFMT_ADDR_WIDTH_LSB)   |
        (QMI_M0_WFMT_SUFFIX_WIDTH_VALUE_Q << QMI_M0_WFMT_SUFFIX_WIDTH_LSB) |
        (QMI_M0_WFMT_DUMMY_WIDTH_VALUE_Q  << QMI_M0_WFMT_DUMMY_WIDTH_LSB)  |
        (QMI_M0_WFMT_DATA_WIDTH_VALUE_Q   << QMI_M0_WFMT_DATA_WIDTH_LSB)   |
        (QMI_M0_WFMT_PREFIX_LEN_VALUE_8   << QMI_M0_WFMT_PREFIX_LEN_LSB);
    qmi_hw->m[1].wcmd = 0x38;

    qmi_hw->direct_csr = 0;
    hw_set_bits(&xip_ctrl_hw->ctrl, XIP_CTRL_WRITABLE_M1_BITS);
    restore_interrupts(saved);
    return size;
}

tlsf_t      g_tlsf      = nullptr;
std::size_t g_total     = 0;
std::size_t g_chip_size = 0;

void retune_timing(uint32_t /*new_khz*/) {
    if (g_chip_size) psram_init_hw(PICOADK_PSRAM_CS_PIN);
}

}  // namespace

#endif  // PICOADK_BOARD_V2

namespace picoadk::Psram {

bool available() {
#if PICOADK_BOARD_V2
    return g_tlsf != nullptr;
#else
    return false;
#endif
}

bool init() {
#if PICOADK_BOARD_V2
    if (g_tlsf) return true;

    g_chip_size = psram_init_hw(PICOADK_PSRAM_CS_PIN);
    if (!g_chip_size) return false;
    System::on_clock_changed(retune_timing);

    uint8_t*    heap_start = __psram_dynamic_start;
    uint8_t*    heap_end   = __psram_dynamic_end;
    std::size_t bytes      = (std::size_t)(heap_end - heap_start);
    if (!bytes) {
        // App didn't apply the linker fragment — use the whole chip.
        heap_start = (uint8_t*)0x11000000;
        bytes      = g_chip_size;
    }

    g_tlsf  = tlsf_create_with_pool(heap_start, bytes);
    g_total = bytes;
    return g_tlsf != nullptr;
#else
    return false;
#endif
}

std::size_t total_bytes()        {
#if PICOADK_BOARD_V2
    return g_total;
#else
    return 0;
#endif
}
std::size_t free_bytes()         {
#if PICOADK_BOARD_V2
    return g_tlsf ? tlsf_free_bytes(g_tlsf) : 0;
#else
    return 0;
#endif
}
std::size_t largest_free_block() {
#if PICOADK_BOARD_V2
    return g_tlsf ? tlsf_largest_free_block(g_tlsf) : 0;
#else
    return 0;
#endif
}

void* alloc(std::size_t bytes, std::size_t align) {
#if PICOADK_BOARD_V2
    if (!g_tlsf) return nullptr;
    return tlsf_aligned_alloc(g_tlsf, align, bytes);
#else
    (void)bytes; (void)align;
    return nullptr;
#endif
}
void free(void* p) {
#if PICOADK_BOARD_V2
    if (g_tlsf && p) tlsf_free(g_tlsf, p);
#else
    (void)p;
#endif
}

}  // namespace picoadk::Psram
