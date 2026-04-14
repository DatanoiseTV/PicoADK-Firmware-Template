// UART (5-pin DIN) MIDI transport. Hardware UART configured for 31250 baud
// 8N1 by default; callers may override the baud rate for non-standard
// transports (e.g. TRS-MIDI-A at higher rates).

#include "picoadk/hal/midi.h"
#include "midi_internal.h"
#include "picoadk_board.h"

#include "hardware/uart.h"
#include "hardware/gpio.h"

namespace picoadk::Midi {

namespace {

uart_inst_t* g_uart = PICOADK_MIDI_UART_INST;

// Running-status MIDI parser state.
uint8_t  g_status      = 0;
uint8_t  g_data1       = 0;
bool     g_have_data1  = false;
uint8_t  g_expect_data = 0;       // bytes still expected after status

uint8_t bytes_for(uint8_t status) {
    switch (status & 0xF0) {
        case 0xC0: case 0xD0: return 1;
        case 0xF0: return 0;             // sysex/realtime — handled separately
        default:   return 2;
    }
}

void feed_byte(uint8_t b) {
    if (b & 0x80) {
        if (b >= 0xF8) {
            // Real-time messages are single-byte and never break running status.
            MidiMessage m{ b, 0, 0, MidiTransport::Uart };
            dispatch(m);
            return;
        }
        g_status      = b;
        g_have_data1  = false;
        g_expect_data = bytes_for(b);
        return;
    }

    if (g_status == 0) return;  // junk before first status byte

    if (g_expect_data == 1) {
        MidiMessage m{ g_status, b, 0, MidiTransport::Uart };
        dispatch(m);
        g_have_data1 = false;
    } else {
        if (!g_have_data1) {
            g_data1 = b;
            g_have_data1 = true;
        } else {
            MidiMessage m{ g_status, g_data1, b, MidiTransport::Uart };
            dispatch(m);
            g_have_data1 = false;
        }
    }
}
}  // namespace

bool uart_init(uint32_t baud) {
    uart_init(g_uart, baud);
    gpio_set_function(PICOADK_PIN_MIDI_UART_RX, GPIO_FUNC_UART);
    gpio_set_function(PICOADK_PIN_MIDI_UART_TX, GPIO_FUNC_UART);
    uart_set_format(g_uart, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(g_uart, true);
    return true;
}

void uart_process() {
    while (uart_is_readable(g_uart)) {
        feed_byte(uart_getc(g_uart));
    }
}

void uart_send(const uint8_t* bytes, std::size_t len) {
    uart_write_blocking(g_uart, bytes, len);
}

}  // namespace picoadk::Midi
