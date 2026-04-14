// Vult runtime — picoadk firmware shim.
//
// The Vult transpiler emits C++ that #includes "vultin.h" for its scalar
// math primitives. We provide them here so the generated DSP code links
// against picoadk's HAL — the implementations route to picoadk::dsp::*
// where it makes sense (so we get the same hw-accelerated paths the rest
// of the firmware uses) and to libfixmath-derived inlines otherwise.

#ifndef VULTIN_H
#define VULTIN_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Vult-generated code uses `static_inline` rather than `static inline`; keep
// the alias so its emitted header compiles cleanly under both GCC and MSVC.
#ifndef static_inline
#  ifdef _MSC_VER
#    define static_inline static __inline
#  else
#    define static_inline static inline
#  endif
#endif

typedef int32_t fix16_t;

float   float_samplerate(void);
fix16_t fix_samplerate(void);

// ---- Type conversion ------------------------------------------------------
static inline float   fix_to_float(fix16_t a) { return (float)a / 0x00010000; }
static inline fix16_t float_to_fix(float a)   { return (fix16_t)(a * 0x00010000); }
static inline fix16_t short_to_fix(int16_t x) { return (0x8000 & x) ? (0xFFFF0000 | x) : x; }
static inline int16_t fix_to_short(fix16_t x) { return (x >= 0x00010000 ? 0x00010000 - 1 : x) / 2; }
static inline float   short_to_float(int16_t x) { return (float)x / 0x00010000; }
static inline float   int_to_float(int a)     { return (float)a; }
static inline int     float_to_int(float a)   { return (int)a; }
static inline fix16_t int_to_fix(int a)       { return a * 0x00010000; }
static inline int     fix_to_int(fix16_t a)   { return a >> 16; }
static inline int     int_clip(int v, int lo, int hi) { return v > hi ? hi : (v < lo ? lo : v); }

// ---- Q16 math -------------------------------------------------------------
static inline fix16_t fix_add(fix16_t x, fix16_t y) { return x + y; }
static inline fix16_t fix_sub(fix16_t x, fix16_t y) { return x - y; }
static inline fix16_t fix_mul(fix16_t x, fix16_t y) { return (fix16_t)(((int64_t)x * y) >> 16); }

#include "pico/divider.h"
static inline fix16_t fix_div(fix16_t a, fix16_t b) {
    if (b == 0) return 0;
    // RP2040 has a hardware divider; on RP2350 div_s64s64 still works (libgcc).
    return (fix16_t)div_s64s64(((int64_t)a) << 16, (int64_t)b);
}

static inline fix16_t fix_mac(fix16_t x, fix16_t y, fix16_t z) { return x + fix_mul(y, z); }
static inline fix16_t fix_msu(fix16_t x, fix16_t y, fix16_t z) { return -x + fix_mul(y, z); }
static inline fix16_t fix_minus(fix16_t x) { return -x; }
static inline fix16_t fix_abs(fix16_t x)   { return x < 0 ? -x : x; }
static inline fix16_t fix_min(fix16_t a, fix16_t b) { return a < b ? a : b; }
static inline fix16_t fix_max(fix16_t a, fix16_t b) { return a > b ? a : b; }
static inline fix16_t fix_clip(fix16_t v, fix16_t lo, fix16_t hi) { return v > hi ? hi : (v < lo ? lo : v); }
static inline fix16_t fix_floor(fix16_t x) { return x & 0xFFFF0000; }
static inline fix16_t fix_not(fix16_t x)   { return ~x; }
static inline float   float_eps()          { return 1e-18f; }
static inline fix16_t fix_eps()            { return 1; }
static inline float   float_pi()           { return 3.1415926535897932384f; }
static inline fix16_t fix_pi()             { return 205887; }
static inline float   float_mac(float x, float y, float z) { return x + (y * z); }
static inline float   float_msu(float x, float y, float z) { return -x + (y * z); }

fix16_t fix_exp (fix16_t in);
fix16_t fix_sin (fix16_t a);
fix16_t fix_cos (fix16_t a);
fix16_t fix_tan (fix16_t a);
fix16_t fix_sinh(fix16_t a);
fix16_t fix_cosh(fix16_t a);
fix16_t fix_tanh(fix16_t a);
fix16_t fix_sqrt(fix16_t v);

// ---- Float ops -----------------------------------------------------------
static inline float float_clip(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

// ---- Array helpers -------------------------------------------------------
static inline void  float_set(float a[], int i, float v)  { a[i] = v; }
static inline float float_get(float a[], int i)           { return a[i]; }
static inline void  fix_set  (fix16_t a[], int i, fix16_t v) { a[i] = v; }
static inline fix16_t fix_get(fix16_t a[], int i)         { return a[i]; }
static inline void  int_set  (int a[], int i, int v)      { a[i] = v; }
static inline int   int_get  (int a[], int i)             { return a[i]; }
static inline void  bool_set (uint8_t a[], int i, uint8_t v) { a[i] = v; }
static inline uint8_t bool_get(uint8_t a[], int i)        { return a[i]; }

void float_init_array(size_t size, float   value, float   data[]);
void int_init_array  (size_t size, int     value, int     data[]);
void bool_init_array (size_t size, uint8_t value, uint8_t data[]);
void fix_init_array  (size_t size, fix16_t value, fix16_t data[]);

void float_copy_array(size_t size, float*   dest, float*   src);
void int_copy_array  (size_t size, int*     dest, int*     src);
void bool_copy_array (size_t size, uint8_t* dest, uint8_t* src);
void fix_copy_array  (size_t size, fix16_t* dest, fix16_t* src);

static inline uint8_t bool_not(uint8_t x) { return !x; }

static inline fix16_t* fix_wrap_array  (const fix16_t x[]) { return (fix16_t*)x; }
static inline float*   float_wrap_array(const float   x[]) { return (float*)x; }

float   float_random(void);
fix16_t fix_random(void);
int     irandom(void);

void float_print (float   value);
void fix_print   (fix16_t value);
void int_print   (int     value);
void string_print(char*   value);
void bool_print  (uint8_t value);

#ifdef __cplusplus
}
#endif

#endif  /* VULTIN_H */
