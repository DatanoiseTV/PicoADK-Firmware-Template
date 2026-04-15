// Host-side stubs for the firmware bits SamplePlayer touches transitively.

#include "picoadk/hal/storage.h"

namespace picoadk::Storage {
bool        init(const StorageConfig&)            { return false; }
bool        mount()                               { return false; }
void        unmount()                             {}
bool        is_mounted()                          { return false; }
SdFs&       fs()                                  { static char d[1]; return *reinterpret_cast<SdFs*>(d); }
std::size_t read_file(const char*, void*, std::size_t)        { return 0; }
bool        write_file(const char*, const void*, std::size_t) { return false; }
uint64_t    card_size_bytes()                     { return 0; }
const char* fs_type_name()                        { return "none"; }
}
