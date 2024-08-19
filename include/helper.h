#ifndef __HELPER_H__
#define __HELPER_H__
#pragma once

#include "vultin.h"

static inline int32_t fix16_to_int32(fix16_t x)
{
    fix16_t out;
    if (x >= int_to_fix(1))
        out = int_to_fix(1) - 1;
    else if (x <= int_to_fix(-1))
        out = int_to_fix(-1) + 1;
    else
        out = x;
    return out << 15u;
}

static inline int32_t float_to_int32(float input) {
    // Simple conversion from float to int32
    // This assumes that the input float is within the range that can be represented by int32
    // Typically, audio samples are normalized between -1.0 and 1.0, so we multiply by the max int32 value
    int32_t output;

    if (input >= 1.0f) {
        output = INT32_MAX;
    } else if (input <= -1.0f) {
        output = INT32_MIN;
    } else {
        output = (int32_t)(input * (float)INT32_MAX);
    }

    return output;
}

static inline int rev_log_scale(int x)
{
    // Calculate reverse logarithmic value from linear input
    return (int)(pow(10, abs(x) / 2048.0) / 10.0 * 2048.0 - 2048.0);
}

#endif // __HELPER_H__