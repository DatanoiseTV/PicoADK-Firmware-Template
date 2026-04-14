// PureData engine bridge — Phase 6.
//
// libpd is added to the build via cmake/picoadk/libpd.cmake when the user
// opts in with -DPICOADK_LIBPD=ON. Until then this file ships a stub so
// applications that *reference* the API still compile (they just can't load
// patches).

#include "picoadk/dsp/pd.h"

#if PICOADK_LIBPD
extern "C" {
#include "z_libpd.h"
}
#endif

namespace picoadk::Pd {

#if PICOADK_LIBPD
namespace {
Config g_cfg;
}

bool init(const Config& cfg) {
    g_cfg = cfg;
    libpd_init();
    return libpd_init_audio(cfg.in_channels, cfg.out_channels, (int)cfg.sample_rate_hz) == 0;
}

int load_patch_from_file(const char* path, const char* dir) {
    void* h = libpd_openfile(path, dir);
    return h ? 1 : 0;
}
int load_patch_from_memory(const void*, std::size_t, const char*) {
    return -1;   // libpd doesn't expose memory-load directly; Phase 6 vendors
                 //   a small RIFF-like loader that writes a tmpfile.
}
void close_patch(int) { /* libpd_closefile would need the void* handle. */ }

void send_bang  (const char* r)              { libpd_bang  (r); }
void send_float (const char* r, float v)     { libpd_float (r, v); }
void send_symbol(const char* r, const char* s){ libpd_symbol(r, s); }
void send_note_on (uint8_t ch, uint8_t n, uint8_t v) { libpd_noteon (ch, n, v); }
void send_note_off(uint8_t ch, uint8_t n)            { libpd_noteon (ch, n, 0); }
void send_cc      (uint8_t ch, uint8_t cc, uint8_t v){ libpd_controlchange(ch, cc, v); }

void on_float (const char*, FloatReceiver)  {}
void on_symbol(const char*, SymbolReceiver) {}
void on_bang  (const char*, BangReceiver)   {}

void process_block(const float* const* in, float* const* out, std::size_t frames) {
    libpd_process_float((int)(frames / g_cfg.block_size), in[0], out[0]);
}

#else
bool init(const Config&)                                              { return false; }
int  load_patch_from_file (const char*, const char*)                  { return 0; }
int  load_patch_from_memory(const void*, std::size_t, const char*)    { return 0; }
void close_patch(int)                                                 {}
void send_bang  (const char*)                                         {}
void send_float (const char*, float)                                  {}
void send_symbol(const char*, const char*)                            {}
void send_note_on (uint8_t, uint8_t, uint8_t)                         {}
void send_note_off(uint8_t, uint8_t)                                  {}
void send_cc      (uint8_t, uint8_t, uint8_t)                         {}
void on_float (const char*, FloatReceiver)                            {}
void on_symbol(const char*, SymbolReceiver)                           {}
void on_bang  (const char*, BangReceiver)                             {}
void process_block(const float* const*, float* const*, std::size_t)   {}
#endif

}  // namespace picoadk::Pd
