// Vult runtime — implementations of the non-inline helpers (trig, exp,
// sqrt, array, random, print). Kept compatible with the original libfixmath-
// derived versions so generated DSP code behaves the same.

#include "vultin.h"
#include <stdio.h>

extern "C" {

float   float_samplerate(void) { return 48000.0f; }
fix16_t fix_samplerate(void)   { return 48000 << 16; }

fix16_t fix_exp(fix16_t inValue) {
    if (inValue == 0)        return 0x00010000;
    if (inValue == 0x00010000) return 178145;
    if (inValue >=  681391)  return 0x7FFFFFFF;
    if (inValue <= -772243)  return 0;
    int neg = inValue < 0;
    if (neg) inValue = -inValue;
    fix16_t result = inValue + 0x00010000;
    fix16_t term   = inValue;
    for (int i = 2; i < 30; ++i) {
        term = fix_mul(term, fix_div(inValue, int_to_fix(i)));
        result += term;
        if (term < 500 && (i > 15 || term < 20)) break;
    }
    if (neg) result = fix_div(0x00010000, result);
    return result;
}

fix16_t fix_sin(fix16_t x0) {
    fix16_t x1 = (x0 % 0x6487e);
    uint8_t sign = (x1 > 0x3243f);
    fix16_t x2 = (x1 % 0x3243f);
    fix16_t x3 = (x2 > 0x1921f) ? fix_add(0x3243f, -x2) : x2;
    fix16_t xp2 = fix_mul(x3, x3);
    fix16_t acc = fix_mul(x3, fix_add(0x10000,
                          fix_mul(fix_add(0xffffd556, fix_mul(0x222, xp2)), xp2)));
    return sign ? -acc : acc;
}

fix16_t fix_cos(fix16_t a)  { return fix_sin(a + (fix_pi() >> 1)); }
fix16_t fix_tan(fix16_t a)  { return fix_div(fix_sin(a), fix_cos(a)); }
fix16_t fix_sinh(fix16_t a) { return fix_mul(fix_exp(a) - fix_exp(-a), 0x8000); }
fix16_t fix_cosh(fix16_t a) { return fix_mul(fix_exp(a) + fix_exp(-a), 0x8000); }
fix16_t fix_tanh(fix16_t a) {
    fix16_t e = fix_exp(a), m = fix_exp(-a);
    return fix_div(e - m, e + m);
}

fix16_t fix_sqrt(fix16_t inValue) {
    uint8_t  neg = inValue < 0;
    uint32_t num = neg ? -inValue : inValue;
    uint32_t result = 0;
    uint32_t bit = (num & 0xFFF00000) ? ((uint32_t)1 << 30) : ((uint32_t)1 << 18);
    while (bit > num) bit >>= 2;
    for (int n = 0; n < 2; ++n) {
        while (bit) {
            if (num >= result + bit) { num -= result + bit; result = (result >> 1) + bit; }
            else                     { result = result >> 1; }
            bit >>= 2;
        }
        if (n == 0) {
            if (num > 65535) {
                num -= result;
                num    = (num    << 16) - 0x8000;
                result = (result << 16) + 0x8000;
            } else {
                num    <<= 16;
                result <<= 16;
            }
            bit = 1 << 14;
        }
    }
    return neg ? -(int32_t)result : (int32_t)result;
}

void float_init_array(size_t n, float   v, float   d[]) { for (size_t i = 0; i < n; ++i) d[i] = v; }
void int_init_array  (size_t n, int     v, int     d[]) { for (size_t i = 0; i < n; ++i) d[i] = v; }
void bool_init_array (size_t n, uint8_t v, uint8_t d[]) { for (size_t i = 0; i < n; ++i) d[i] = v; }
void fix_init_array  (size_t n, fix16_t v, fix16_t d[]) { for (size_t i = 0; i < n; ++i) d[i] = v; }

void float_copy_array(size_t n, float*   dst, float*   src) { for (size_t i = 0; i < n; ++i) dst[i] = src[i]; }
void int_copy_array  (size_t n, int*     dst, int*     src) { for (size_t i = 0; i < n; ++i) dst[i] = src[i]; }
void bool_copy_array (size_t n, uint8_t* dst, uint8_t* src) { for (size_t i = 0; i < n; ++i) dst[i] = src[i]; }
void fix_copy_array  (size_t n, fix16_t* dst, fix16_t* src) { for (size_t i = 0; i < n; ++i) dst[i] = src[i]; }

float   float_random(void) { return (float)rand() / RAND_MAX; }
fix16_t fix_random  (void) { return (fix16_t)(((float)rand() / RAND_MAX) * 0x00010000); }
int     irandom     (void) { return rand(); }

void float_print (float   v) { printf("%f\n",  (double)v); }
void fix_print   (fix16_t v) { printf("%f\n",  (double)fix_to_float(v)); }
void int_print   (int     v) { printf("%i\n",  v); }
void string_print(char*   v) { printf("%s\n",  v); }
void bool_print  (uint8_t v) { printf("%s\n",  v ? "true" : "false"); }

}  // extern "C"

// Vult external — `external adc128_read_vult(int) : real;`
extern "C" {
#include "picoadk_board.h"
}
namespace picoadk { namespace Controls { uint16_t read_adc8(uint8_t); } }

extern "C" fix16_t adc128_read_vult(int chan) {
    if (chan < 0 || chan > 7) return 0;
    uint16_t v = picoadk::Controls::read_adc8((uint8_t)chan);
    return (fix16_t)(v << 4);   // 12-bit → fix16 by left-shifting into the fractional part
}
