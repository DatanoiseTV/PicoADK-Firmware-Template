// TinySoundFont — single translation unit that defines the implementation.
// Compiled as plain C so the various <stdlib.h>-flavoured includes inside
// TSF don't clash with libstdc++'s `<cstdlib>`.

#define TSF_IMPLEMENTATION
#define TSF_NO_STDIO     /* we feed buffers, not FILE* */

#include "tsf.h"
