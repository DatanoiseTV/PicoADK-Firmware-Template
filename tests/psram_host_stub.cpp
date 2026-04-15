// Host-side stubs for the firmware bits the DSP modules might link against
// (PSRAM allocator). The host tests never use PSRAM, so we just punt to the
// system heap.

#include "picoadk/hal/psram.h"
#include <cstdlib>

namespace picoadk::Psram {
bool        available()                                  { return false; }
bool        init()                                       { return false; }
std::size_t total_bytes()                                { return 0; }
std::size_t free_bytes()                                 { return 0; }
std::size_t largest_free_block()                         { return 0; }
void*       alloc(std::size_t n, std::size_t /*align*/)  { return std::malloc(n); }
void        free(void* p)                                { std::free(p); }
}
