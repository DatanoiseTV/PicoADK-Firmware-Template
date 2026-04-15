// PSRAM stub — RP2040 has no QMI-attached PSRAM, so all calls fall back to
// the system heap. Keeping this stub means user code that uses PSRAM_BSS
// macros and Psram::alloc still builds and runs on v1 (just placing data in
// regular SRAM).

#include "picoadk/hal/psram.h"
#include <cstdlib>

namespace picoadk::Psram {

bool        available()         { return false; }
bool        init()              { return false; }
std::size_t total_bytes()       { return 0; }
std::size_t free_bytes()        { return 0; }
std::size_t largest_free_block(){ return 0; }
void*       alloc(std::size_t n, std::size_t /*align*/) { return std::malloc(n); }
void        free(void* p)       { std::free(p); }

}  // namespace picoadk::Psram
