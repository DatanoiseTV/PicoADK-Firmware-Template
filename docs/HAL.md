# HAL Reference

Per-subsystem reference for `<picoadk/hal/*.h>` and `<picoadk/dsp/*.h>`. Read [docs/ARCHITECTURE.md](ARCHITECTURE.md) first for the big-picture model.

Every subsystem follows the same pattern:

```cpp
// 1. Configure
SubsystemConfig cfg;
cfg.thing = ...;

// 2. Bring up
Subsystem::init(cfg);

// 3. Use
Subsystem::do_thing();
```

`Hardware::init(HardwareConfig)` brings them all up at once with sensible defaults — the snippets below show what each one does individually.

---

## System  — `picoadk/hal/system.h`

```cpp
SystemConfig cfg;
cfg.sys_clock_khz       = 0;          // 0 = board default (RP2040: 402 MHz, RP2350: 150 MHz)
cfg.enable_watchdog     = true;
cfg.watchdog_timeout_ms = 8000;
cfg.enable_stdio_uart   = true;
System::init(cfg);

System::feed_watchdog();      // call from a task >= every 4 s
auto t  = System::micros();
auto ms = System::millis();
const char* name = System::board_name();   // "PicoADK v1 (RP2040)" or v2

// ESP-IDF style core-pinned task (SMP on both boards):
xTaskCreatePinnedToCore(my_task, "AUDIO", 2048, nullptr, 4, &handle, 0);
```

Stdio routes to UART by default. To get printf over USB-CDC, enable `UsbClass::Cdc` in your `UsbConfig`; the system layer then registers a CDC stdio driver alongside UART.

---

## Audio  — `picoadk/hal/audio.h`

```cpp
AudioConfig cfg;
cfg.sample_rate_hz   = 48000;
cfg.bit_depth        = AudioBitDepth::Bits24;
cfg.block_size       = 32;                       // frames per callback
cfg.num_channels     = 2;
cfg.direction        = AudioDirection::Duplex;   // Out / In / Duplex
cfg.frame_format     = AudioFrameFormat::I2S;    // or LeftJust / RightJust / TDM
cfg.slots_per_frame  = 2;                        // 2 / 4 / 8
cfg.slot_width       = 32;
cfg.bclk_polarity    = AudioPolarity::Normal;
cfg.ws_polarity      = AudioPolarity::Normal;
Audio::init(cfg);

Audio::set_callback([](const float* const* in, float* const* out, std::size_t frames, void*) {
    // in is nullptr when direction == Out
    // out is planar: out[0] is left, out[1] is right (etc. for TDM slots)
    for (std::size_t i = 0; i < frames; ++i) out[0][i] = out[1][i] = 0.0f;
});

Audio::start();              // kicks the DMA chain
float load = Audio::load();  // last-block CPU load, 0..1+
Audio::stop();
Audio::reconfigure(cfg);     // safe live SR / direction change
```

| Direction | TX SM | RX SM |
|---|---|---|
| `Out`    | yes | no  |
| `In`     | no  | yes |
| `Duplex` | yes | yes (shares BCK/WS) |

---

## MIDI  — `picoadk/hal/midi.h` + `picoadk/midi/general_handler.h`

Low-level: per-message callbacks, transport-aware.

```cpp
MidiConfig cfg;
cfg.inputs    = MidiTransport::Usb | MidiTransport::Uart | MidiTransport::UsbHost;
cfg.outputs   = MidiTransport::Usb;
cfg.uart_baud = 31250;
Midi::init(cfg);

Midi::set_note_on  ([](uint8_t ch, uint8_t n, uint8_t v) { /* ... */ });
Midi::set_note_off ([](uint8_t ch, uint8_t n, uint8_t v) { /* ... */ });
Midi::set_cc       ([](uint8_t ch, uint8_t cc, uint8_t v) { /* ... */ });
Midi::set_pitch_bend([](uint8_t ch, int16_t v14) { /* ... */ });
Midi::set_raw      ([](const MidiMessage& m) { /* m.source == Usb/Uart/UsbHost */ });

while (true) Midi::process();        // pump from a task

Midi::send_note_on(0, 60, 100);
```

High-level: NRPN/RPN aggregation, 14-bit pitch bend, MIDI clock + transport, channel-mode messages.

```cpp
midi::Handler h;
h.on_note_on        = [](uint8_t, uint8_t n, uint8_t v) { /* ... */ };
h.on_pitch_bend     = [](uint8_t, float semis) { /* ... */ };
h.on_rpn            = [](uint8_t, uint16_t num, uint16_t val) { /* ... */ };
h.on_clock_tick     = []() { /* ... */ };
h.pitch_bend_range_semis = 12.0f;
midi::Handler_ns::install(&h);
```

Launchpad programmer-mode driver — `picoadk/midi/launchpad.h`. Diff-only updates: only changed pads are sent.

---

## USB  — `picoadk/hal/usb_device.h` + `picoadk/hal/usb_host.h`

Compose the device descriptor at runtime:

```cpp
UsbConfig cfg;
cfg.classes         = UsbClass::Cdc | UsbClass::Midi | UsbClass::Msc;
cfg.midi_cables     = 4;
cfg.msc_backend     = UsbMscBackend::SdCard;     // or FlashFs / Both
cfg.cdc_routes_stdio= true;
cfg.product         = "MyDevice";
cfg.vid             = 0x2E8A;
cfg.pid             = 0x104B;
Usb::configure(cfg);                  // call BEFORE Hardware::init / System::init
```

Endpoint numbers are fixed per class so adding/removing classes doesn't renumber and confuse drivers.

USB host (Phase 1e) — pico_pio_usb second-port host so the native port stays in device mode:

```cpp
UsbHostConfig hcfg;
hcfg.use_pio_usb        = true;
hcfg.pio_usb_dp_pin     = 16;       // D+ on this pin, D- on (pin+1)
hcfg.midi_class_enabled = true;
UsbHost::init(hcfg);

while (true) UsbHost::process();    // host-side MIDI lands in regular Midi callbacks
```

---

## Controls  — `picoadk/hal/controls.h`, `encoders.h`, `buttons.h`, `mux.h`

```cpp
Controls::init();
uint16_t v   = Controls::read_adc8(0);            // MCP3208 channel 0 (12-bit)
float    v01 = Controls::read_adc8_norm(0);
Controls::set_status_leds(0b0101);
Controls::set_note_led(true);

EncoderPins ep[2] = {
    { .pin_a = 7, .pin_b = 8, .pin_btn = 6 },
    { .pin_a = 9, .pin_b = 10 },
};
Encoders::init(ep, 2);
EncoderReading enc[2];
Encoders::poll(enc, 2);
if (enc[0].delta) cursor += enc[0].delta;

ButtonPins bp[3] = {
    { .pin = 14, .debounce_ms = 8, .hold_ms = 600, .double_click_ms = 250 },
};
Buttons::init(bp, 3);
ButtonReading br[3];
Buttons::poll(br, 3);
if (br[0].event == ButtonEvent::DoubleClicked) ...

// Mux scanner — mix CD4051 banks with direct pots in one scan table.
MuxBank bank;
bank.addr_pins[0] = 2; bank.addr_pins[1] = 3; bank.addr_pins[2] = 4;
bank.addr_count   = 3; bank.num_channels = 8;
bank.adc_source   = AdcSource::Mcp3208; bank.adc_channel = 0;
DirectInput direct = { AdcSource::BoardAdc, 0 };
ScannerConfig sc{ &bank, 1, &direct, 1, 1000 /* Hz */ };
auto total = Scanner::init(sc);              // returns # of inputs
float v = Scanner::read_norm(0);              // 0..total-1
```

---

## Display  — `picoadk/hal/display.h` + `gfx.h`

```cpp
DisplayConfig cfg;
cfg.driver       = DisplayDriver::SSD1306_128x64;
cfg.bus          = DisplayBus::I2C;
cfg.i2c_address  = 0x3C;
cfg.i2c_baud_hz  = 1'000'000;        // SSD1306 happy at FM+
Display::init(cfg);

// gfx::Canvas wraps the common drawing calls
auto& g = Display::canvas();
g.clear();
g.text(0, 12, "PicoADK v3", u8g2_font_6x10_tr);
g.line(0, 16, 127, 16);
g.fill_rect(10, 20, 30, 10);
g.circle(80, 30, 8);
Display::present();                  // DMA push to the panel

// Need fancier u8g2 features? Reach for the underlying instance:
u8g2_t& u = Display::u8g2();
u8g2_SetFont(&u, u8g2_font_helvB14_te);
u8g2_DrawUTF8(&u, 0, 60, "中文 ñ");
Display::present();
```

---

## Storage  — `picoadk/hal/storage.h`

```cpp
StorageConfig cfg;
cfg.bus       = StorageBus::Auto;          // Auto: SDIO on v2, SPI on v1
cfg.clock_khz = 0;                          // 0 = max safe
Storage::init(cfg);

if (Storage::is_mounted()) {
    auto bytes = Storage::card_size_bytes();
    auto fs    = Storage::fs_type_name();   // "FAT32" / "exFAT" / ...

    // Convenience helpers:
    uint8_t buf[512];
    Storage::read_file ("/preset.bin", buf, sizeof(buf));
    Storage::write_file("/log.txt", "hello\n", 6);

    // Or talk SdFat directly for streaming:
    SdFs& fs = Storage::fs();
    FsFile f;
    f.open("/big.wav", O_RDONLY);
    f.read(buf, sizeof(buf));
    f.close();
}
```

Build with `-DPICOADK_ENABLE_SDFAT=ON` (and `-DPICOADK_ENABLE_SDIO=ON` on v2 for full-speed SDIO).

---

## PSRAM  — `picoadk/hal/psram.h`

```cpp
Psram::init();   // also called by Hardware::init() automatically
if (Psram::available()) {
    auto* big   = (float*)Psram::alloc(2 * 1024 * 1024, 8);
    auto* fancy = new (PSRAM) MyClass{...};         // placement-new
    Psram::free(big);
}

// Linker-reserved sections (data placed at boot):
PICOADK_PSRAM_BSS  float wavetable[1 << 16];        // BSS-style (zero-init)
PICOADK_PSRAM_DATA float lookup[256] = { ... };     // initialised data
```

On v1 (RP2040) every call falls back to the system heap and the section macros are no-ops, so portable code Just Works.

---

## VoiceLink  — `picoadk/hal/voicelink.h`

Distributed polyphony across multiple boards.

```cpp
// MASTER firmware
VoiceLinkConfig vc;
vc.role         = VoiceLinkRole::Master;
vc.uart_index   = 1;
vc.tx_pin       = 4; vc.rx_pin = 5;
vc.baud         = 3'000'000;
vc.voices_local = 8;       // master can play 8 itself
vc.alloc        = VoiceAllocPolicy::Quietest;
VoiceLink::init(vc);

VoiceLink::on_note_on  ([](uint8_t v, uint8_t c, uint8_t n, uint8_t vel){ /* play locally */ });
VoiceLink::on_note_off ([](uint8_t v, uint8_t c, uint8_t n, uint8_t vel){ /* ... */ });

// Application route — fan a note out to whichever slot the allocator picks
Midi::set_note_on([](uint8_t c, uint8_t n, uint8_t v){ VoiceLink::note_on(c, n, v); });

// SLAVE firmware uses VoiceLinkRole::Slave; same callbacks fire when the
// master sends a frame addressed at this slot.
```

Per-card capacity is reported during enumeration; `VoiceLink::total_polyphony()` gives you the chain-wide voice count for UI display.

---

## DSP modules  — `picoadk/dsp/*.h`

`Real` adapts: `float` on RP2350 (FPU), Q16 fixed-point on RP2040.

```cpp
using namespace picoadk::dsp;
constexpr float SR = 48000.0f;

BlepOscillator osc;       osc.reset(SR); osc.set_shape(BlepOscillator::Shape::Saw); osc.set_freq_hz(220.f);
StateVariableFilter svf;  svf.reset(SR); svf.set_freq_hz(800.f); svf.set_q(0.7f);
Adsr env;                  env.reset(SR); env.set(0.01f, 0.2f, 0.7f, 0.5f); env.gate(true);
Compressor comp;           comp.reset(SR); comp.set(/*thr*/-12.f, /*ratio*/4.f, /*atk*/3.f, /*rel*/80.f);

// PSRAM-backed FX for v2
DelayLine d;  d.configure_auto(/*sec*/1.5f, SR);  d.set_time_seconds(0.25f, SR); d.set_feedback(0.5f);
FdnReverb r;  r.configure(/*decay*/3.5f, SR);  r.set_size(0.7f); r.set_damping(0.4f);

// Inside the audio callback
Real x = osc.process();
x = svf.process(x);
x = mul(x, env.process());
out_l[i] = out_r[i] = comp.process(d.process(x));
```

---

## Sampler — `picoadk/dsp/sample_player.h` + `sf2.h`

```cpp
// Memory-resident
auto* mem = MemorySampleSource::load_wav_from_sd("/kick.wav");
SamplePlayer p;  p.reset(SR);  p.set_source(mem);  p.trigger();
p.set_pitch_semitones(+5.f);
p.set_loop(true, /*start*/0, /*end*/mem->length());

// Streaming with prefetch ring (PSRAM-backed)
auto* str = StreamingSampleSource::open_wav("/long_sample.wav");
SamplePlayer s;  s.reset(SR);  s.set_source(str);  s.trigger();

// Multisample keymap
KeyZone zones[3] = {
    { mem_low,  24, 47, 36, 1.0f },
    { mem_mid,  48, 71, 60, 1.0f },
    { mem_high, 72, 95, 84, 1.0f },
};
MultisamplePlayer ms;  ms.reset(SR, /*voices*/8);  ms.set_zones(zones, 3);
ms.note_on(60, 100);   // → picks the nearest zone, pitches to root, allocates a voice

// SoundFont 2 — full multisample/loop/release/modulators via TinySoundFont
Sf2Player sf2;
sf2.load_from_sd("/grand.sf2");
sf2.set_sample_rate(SR);
sf2.set_preset(0, 0);
sf2.note_on(0, 60, 1.0f);

// SFZ — embedded subset
SfzPlayer sfz;
sfz.load_from_sd("/library.sfz", /*voices*/12);
sfz.note_on(0, 60, 100);
```

`Sf2Player::process(out_l, out_r, frames)` and `SfzPlayer::process(...)` are additive — call them from the audio callback after zeroing the output buffers (or other generators write first).

---

## See also

* [docs/ARCHITECTURE.md](ARCHITECTURE.md) — layered model, audio pipeline, threading, memory map.
* [examples/](../examples/) — six runnable apps that exercise every subsystem above.
* [include/picoadk/picoadk.h](../include/picoadk/picoadk.h) — umbrella header listing every public include.
