#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>

/* Q16.16: 16 integer bits (signed) and 16 fraction bits in an int32_t.
 * Every operation saturates instead of wrapping and rounds to nearest,
 * ties away from zero. No floating point anywhere. */
typedef int32_t fx_t;

#define FX_SHIFT 16
#define FX_ONE ((fx_t)1 << FX_SHIFT)
#define FX_MAX INT32_MAX
#define FX_MIN INT32_MIN

fx_t fx_from_int(int32_t v);
fx_t fx_from_ratio(int32_t num, int32_t den);
int32_t fx_floor(fx_t v);
int32_t fx_round(fx_t v);

fx_t fx_add(fx_t a, fx_t b);
fx_t fx_sub(fx_t a, fx_t b);
fx_t fx_mul(fx_t a, fx_t b);
fx_t fx_div(fx_t a, fx_t b);
fx_t fx_abs(fx_t v);
fx_t fx_sqrt(fx_t v);

#endif
