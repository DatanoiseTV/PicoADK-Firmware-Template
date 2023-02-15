#ifndef __ARDUINO_COMPAT_H__
#define __ARDUINO_COMPAT_H__

#include "picoadk_hw.h"

// Basic Arduino compatibility layer
// This is not a complete implementation of the Arduino API, but it
// provides enough to get the code to compile and run.

// Arduino types
typedef unsigned char byte;
typedef unsigned int word;

// Arduino functions
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define map(x, in_min, in_max, out_min, out_max) ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define delay(x) sleep_ms(x)
#define delayMicroseconds(x) sleep_us(x)
#define millis() to_ms_since_boot(get_absolute_time())
#define micros() to_us_since_boot(get_absolute_time())

#define HIGH 1
#define LOW 0

#define INPUT 0
#define OUTPUT 1

#define pinMode(x,y) gpio_set_dir(x, y)
#define digitalWrite(x,y) gpio_put(x, y)
#define digitalRead(x) gpio_get(x)

static inline uint16_t analogRead(uint8_t pin)
{
    if (pin < 26 || pin > 29)
        return 0;
    adc_select_input(pin);
    adc_run(true);
    sleep_ms(1);
    return adc_read();
}

#endif // __ARDUINO_COMPAT_H__