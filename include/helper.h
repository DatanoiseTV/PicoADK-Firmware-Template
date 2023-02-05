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

static inline int rev_log_scale(int x)
{
    // Calculate reverse logarithmic value from linear input
    return (int)(pow(10, abs(x) / 2048.0) / 10.0 * 2048.0 - 2048.0);
}

#endif // __HELPER_H__