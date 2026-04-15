// MIDI HAL.
//
// Multiple transports can be enabled simultaneously. Inbound messages from
// any source land in a single callback set; outbound messages go to whichever
// transports are configured for output (default: all enabled).

#pragma once

#include <cstddef>
#include <cstdint>

namespace picoadk {

enum class MidiTransport : uint8_t {
    None    = 0,
    Usb     = 1u << 0,    // USB device mode (TinyUSB tud_midi_*)
    Uart    = 1u << 1,    // 5-pin DIN / TRS
    UsbHost = 1u << 2,    // USB host mode (TinyUSB tuh_midi_*) — see UsbHost API
    All     = Usb | Uart | UsbHost,
};

inline constexpr MidiTransport operator|(MidiTransport a, MidiTransport b) {
    return static_cast<MidiTransport>(
        static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
inline constexpr bool any(MidiTransport a, MidiTransport b) {
    return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
}

struct MidiConfig {
    MidiTransport inputs   = MidiTransport::Usb;
    MidiTransport outputs  = MidiTransport::Usb;
    uint32_t      uart_baud = 31250;          // standard 5-pin DIN rate
};

struct MidiMessage {
    uint8_t status;     // includes channel in low nibble for channel msgs
    uint8_t data1;
    uint8_t data2;
    MidiTransport source;
};

using MidiNoteCallback   = void (*)(uint8_t channel, uint8_t note, uint8_t velocity);
using MidiCcCallback     = void (*)(uint8_t channel, uint8_t cc,   uint8_t value);
using MidiPbCallback     = void (*)(uint8_t channel, int16_t value14);
using MidiRawCallback    = void (*)(const MidiMessage& msg);

namespace Midi {

bool init(const MidiConfig& cfg);

void set_note_on (MidiNoteCallback cb);
void set_note_off(MidiNoteCallback cb);
void set_cc      (MidiCcCallback   cb);
void set_pitch_bend(MidiPbCallback cb);
void set_raw     (MidiRawCallback  cb);   // catch-all, fires for everything

// Pump pending input. Call from a FreeRTOS task; not safe from ISRs.
void process();

// Send.  `transports` defaults to whatever the config says.
void send_note_on (uint8_t channel, uint8_t note, uint8_t velocity,
                   MidiTransport transports = MidiTransport::None);
void send_note_off(uint8_t channel, uint8_t note, uint8_t velocity = 0,
                   MidiTransport transports = MidiTransport::None);
void send_cc      (uint8_t channel, uint8_t cc, uint8_t value,
                   MidiTransport transports = MidiTransport::None);
void send_raw     (const uint8_t* bytes, std::size_t len,
                   MidiTransport transports = MidiTransport::None);

const MidiConfig& config();

}  // namespace Midi
}  // namespace picoadk
