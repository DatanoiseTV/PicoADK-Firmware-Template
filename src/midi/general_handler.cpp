// General MIDI handler implementation.
//
// Translates raw MidiMessage stream into typed callbacks (note on/off,
// CC, program change, channel pressure, poly aftertouch, pitch bend with
// 14-bit precision and configurable bend range, MIDI clock + transport,
// RPN/NRPN aggregation).

#include "picoadk/midi/general_handler.h"
#include "midi_internal.h"

namespace picoadk::midi {

namespace {

Handler* g_h = nullptr;

// Per-channel RPN/NRPN accumulator state.
struct ChannelState {
    uint16_t rpn   = 0x3FFF;       // 0x3FFF = "no RPN selected" (sentinel)
    uint16_t nrpn  = 0x3FFF;
    bool     in_nrpn = false;       // most-recent select was an NRPN
};
ChannelState g_ch[16];

bool channel_passes(uint8_t ch) {
    if (!g_h || g_h->channel_mask == 0) return true;
    return (g_h->channel_mask & (1u << ch)) != 0;
}

void emit_rpn(uint8_t ch, uint16_t value14) {
    if (!g_h) return;
    if (g_ch[ch].in_nrpn) {
        if (g_h->on_nrpn) g_h->on_nrpn(ch, g_ch[ch].nrpn, value14);
    } else {
        if (g_h->on_rpn)  g_h->on_rpn (ch, g_ch[ch].rpn,  value14);
    }
}

void on_cc(uint8_t ch, uint8_t cc, uint8_t v) {
    if (!g_h) return;
    if (!channel_passes(ch)) return;

    static uint8_t data_msb[16] = {0};
    switch (cc) {
        case 99: g_ch[ch].nrpn = (uint16_t)((v << 7) | (g_ch[ch].nrpn & 0x7F)); g_ch[ch].in_nrpn = true;  break;
        case 98: g_ch[ch].nrpn = (uint16_t)((g_ch[ch].nrpn & 0x3F80) | v);      g_ch[ch].in_nrpn = true;  break;
        case 101:g_ch[ch].rpn  = (uint16_t)((v << 7) | (g_ch[ch].rpn  & 0x7F)); g_ch[ch].in_nrpn = false; break;
        case 100:g_ch[ch].rpn  = (uint16_t)((g_ch[ch].rpn  & 0x3F80) | v);      g_ch[ch].in_nrpn = false; break;
        case 6:  data_msb[ch] = v; break;                              // data entry MSB — wait for LSB
        case 38: emit_rpn(ch, (uint16_t)((data_msb[ch] << 7) | v));    // data entry LSB → emit
                 break;
        case 96: emit_rpn(ch, (g_ch[ch].in_nrpn ? g_ch[ch].nrpn : g_ch[ch].rpn) + 1); break;  // increment
        case 97: emit_rpn(ch, (g_ch[ch].in_nrpn ? g_ch[ch].nrpn : g_ch[ch].rpn) - 1); break;
        case 120: if (g_h->on_all_sound_off)     g_h->on_all_sound_off(ch);     return;
        case 121: if (g_h->on_reset_controllers) g_h->on_reset_controllers(ch); return;
        case 123: if (g_h->on_all_notes_off)     g_h->on_all_notes_off(ch);     return;
        default: break;
    }
    if (g_h->on_cc) g_h->on_cc(ch, cc, v);
}

void on_message(const MidiMessage& m) {
    if (!g_h) return;
    if (g_h->on_raw) g_h->on_raw(m);

    uint8_t kind = m.status & 0xF0;
    uint8_t ch   = m.status & 0x0F;

    // System real-time / common
    if ((m.status & 0xF0) == 0xF0) {
        switch (m.status) {
            case 0xF8: if (g_h->on_clock_tick) g_h->on_clock_tick(); break;
            case 0xFA: if (g_h->on_start)      g_h->on_start();      break;
            case 0xFB: if (g_h->on_continue)   g_h->on_continue();   break;
            case 0xFC: if (g_h->on_stop)       g_h->on_stop();       break;
            case 0xF2: if (g_h->on_song_position)
                          g_h->on_song_position((uint16_t)((m.data2 << 7) | m.data1));
                       break;
        }
        return;
    }
    if (!channel_passes(ch)) return;

    switch (kind) {
        case 0x80: if (g_h->on_note_off) g_h->on_note_off(ch, m.data1, m.data2); break;
        case 0x90:
            if (m.data2 == 0) { if (g_h->on_note_off) g_h->on_note_off(ch, m.data1, 0); }
            else              { if (g_h->on_note_on)  g_h->on_note_on (ch, m.data1, m.data2); }
            break;
        case 0xA0: if (g_h->on_poly_aftertouch) g_h->on_poly_aftertouch(ch, m.data1, m.data2); break;
        case 0xB0: on_cc(ch, m.data1, m.data2); break;
        case 0xC0: if (g_h->on_program_change)  g_h->on_program_change (ch, m.data1); break;
        case 0xD0: if (g_h->on_channel_pressure)g_h->on_channel_pressure(ch, m.data1); break;
        case 0xE0: {
            int16_t v14 = (int16_t)(((m.data2 << 7) | m.data1) - 8192);
            if (g_h->on_pitch_bend) {
                float semis = (float)v14 * (g_h->pitch_bend_range_semis / 8192.0f);
                g_h->on_pitch_bend(ch, semis);
            }
        } break;
    }
}

}  // namespace

namespace Handler_ns {
void install(Handler* h) {
    g_h = h;
    Midi::set_raw([](const MidiMessage& m) { on_message(m); });
}
}  // namespace Handler_ns

}  // namespace picoadk::midi
