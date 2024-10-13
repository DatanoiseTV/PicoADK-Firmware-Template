#ifndef __PICOADK_HW_H__
#define __PICOADK_HW_H__

#include <stdio.h>
#include <cstdlib>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "hardware/structs/rosc.h"
#include "helper.h"
#include "psram.h"

#if __has_include("bsp/board_api.h")
#include "bsp/board_api.h"
#else
#include "bsp/board.h"
#endif

void picoadk_init();
int adc128_read(uint8_t chan);

#endif // __PICOADK_HW_H__
