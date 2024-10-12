#ifndef __AUDIO_SUBSYSTEM_H__
#define __AUDIO_SUBSYSTEM_H__


#define SAMPLES_PER_BUFFER 64 // Samples / channel

#define USE_AUDIO_I2S 1
#include "audio_i2s.h"
#include <stdio.h>

audio_buffer_pool_t *init_audio();

#endif // __AUDIO_SUBSYSTEM_H__
