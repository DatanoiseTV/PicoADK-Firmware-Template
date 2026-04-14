// MIDI HAL — multi-transport dispatcher.
//
// Each enabled transport (USB, UART) runs independently and feeds parsed
// messages into the unified callback set. Phase 1 fills the transports in;
// this file owns the dispatcher.

#include "picoadk/hal/midi.h"

namespace picoadk::Midi {

extern bool usb_init();
extern void usb_process();
extern void usb_send (const uint8_t* bytes, std::size_t len);

extern bool uart_init(uint32_t baud);
extern void uart_process();
extern void uart_send(const uint8_t* bytes, std::size_t len);

namespace {

MidiConfig         g_cfg;
MidiNoteCallback   g_on_on  = nullptr;
MidiNoteCallback   g_on_off = nullptr;
MidiCcCallback     g_on_cc  = nullptr;
MidiPbCallback     g_on_pb  = nullptr;
MidiRawCallback    g_on_raw = nullptr;

}  // namespace

bool init(const MidiConfig& cfg) {
    g_cfg = cfg;
    bool ok = true;
    if (any(cfg.inputs, MidiTransport::Usb) || any(cfg.outputs, MidiTransport::Usb)) {
        ok &= usb_init();
    }
    if (any(cfg.inputs, MidiTransport::Uart) || any(cfg.outputs, MidiTransport::Uart)) {
        ok &= uart_init(cfg.uart_baud);
    }
    return ok;
}

void set_note_on  (MidiNoteCallback cb) { g_on_on  = cb; }
void set_note_off (MidiNoteCallback cb) { g_on_off = cb; }
void set_cc       (MidiCcCallback   cb) { g_on_cc  = cb; }
void set_pitch_bend(MidiPbCallback  cb) { g_on_pb  = cb; }
void set_raw      (MidiRawCallback  cb) { g_on_raw = cb; }

void process() {
    if (any(g_cfg.inputs, MidiTransport::Usb))  usb_process();
    if (any(g_cfg.inputs, MidiTransport::Uart)) uart_process();
}

namespace {
void send_to(MidiTransport mask, const uint8_t* b, std::size_t n) {
    if (any(mask, MidiTransport::Usb))  usb_send (b, n);
    if (any(mask, MidiTransport::Uart)) uart_send(b, n);
}
}

void send_note_on(uint8_t ch, uint8_t note, uint8_t v, MidiTransport t) {
    if (t == MidiTransport::None) t = g_cfg.outputs;
    uint8_t b[3] = { uint8_t(0x90 | (ch & 0x0F)), uint8_t(note & 0x7F), uint8_t(v & 0x7F) };
    send_to(t, b, 3);
}
void send_note_off(uint8_t ch, uint8_t note, uint8_t v, MidiTransport t) {
    if (t == MidiTransport::None) t = g_cfg.outputs;
    uint8_t b[3] = { uint8_t(0x80 | (ch & 0x0F)), uint8_t(note & 0x7F), uint8_t(v & 0x7F) };
    send_to(t, b, 3);
}
void send_cc(uint8_t ch, uint8_t cc, uint8_t v, MidiTransport t) {
    if (t == MidiTransport::None) t = g_cfg.outputs;
    uint8_t b[3] = { uint8_t(0xB0 | (ch & 0x0F)), uint8_t(cc & 0x7F), uint8_t(v & 0x7F) };
    send_to(t, b, 3);
}
void send_raw(const uint8_t* bytes, std::size_t len, MidiTransport t) {
    if (t == MidiTransport::None) t = g_cfg.outputs;
    send_to(t, bytes, len);
}

const MidiConfig& config() { return g_cfg; }

// Internal dispatch helper used by transports when they parse a message.
void dispatch(const MidiMessage& m) {
    if (g_on_raw) g_on_raw(m);
    uint8_t kind = m.status & 0xF0;
    uint8_t ch   = m.status & 0x0F;
    switch (kind) {
        case 0x80: if (g_on_off) g_on_off(ch, m.data1, m.data2); break;
        case 0x90:
            if (m.data2 == 0) { if (g_on_off) g_on_off(ch, m.data1, 0); }
            else              { if (g_on_on)  g_on_on (ch, m.data1, m.data2); }
            break;
        case 0xB0: if (g_on_cc) g_on_cc(ch, m.data1, m.data2); break;
        case 0xE0: if (g_on_pb) g_on_pb(ch, int16_t((int(m.data2) << 7) | m.data1) - 8192); break;
        default: break;
    }
}

}  // namespace picoadk::Midi
