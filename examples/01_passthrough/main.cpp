// =============================================================================
//  PicoADK v3 — Audio Passthrough
// -----------------------------------------------------------------------------
//  The smallest possible scaffold: bring up the HAL, configure full-duplex
//  I2S, copy input to output. ~25 lines of business logic.
// =============================================================================

#include "picoadk/picoadk.h"

#include <FreeRTOS.h>
#include <task.h>
#include <cstring>

using namespace picoadk;

void passthrough(const float* const* in, float* const* out, std::size_t frames, void*) {
    if (!in) {                              // output-only build → silence
        std::memset(out[0], 0, frames * sizeof(float));
        std::memset(out[1], 0, frames * sizeof(float));
        return;
    }
    std::memcpy(out[0], in[0], frames * sizeof(float));
    std::memcpy(out[1], in[1], frames * sizeof(float));
}

int main() {
    HardwareConfig hw;
    hw.audio.sample_rate_hz = 48000;
    hw.audio.block_size     = 64;
    hw.audio.direction      = AudioDirection::Duplex;
    Hardware::init(hw);

    Audio::set_callback(passthrough);
    Audio::start();

    vTaskStartScheduler();
    for (;;) __asm volatile("wfi");
}
