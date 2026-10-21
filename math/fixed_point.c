#include "fixed_point.h"

static fx_t saturate(int64_t v) {
    if (v > FX_MAX) {
        return FX_MAX;
    }
    if (v < FX_MIN) {
        return FX_MIN;
    }
    return (fx_t)v;
}

/* Truncating division plus a correction keeps the rounding rule explicit;
 * a right shift of a negative value is implementation-defined in C. */
static int64_t div_round(int64_t n, int64_t d) {
    int64_t q = n / d;
    int64_t r = n % d;
    int64_t ar = r < 0 ? -r : r;
    int64_t ad = d < 0 ? -d : d;
    if (2 * ar >= ad) {
        q += ((n < 0) != (d < 0)) ? -1 : 1;
    }
    return q;
}

fx_t fx_from_int(int32_t v) {
    return saturate((int64_t)v * FX_ONE);
}

fx_t fx_from_ratio(int32_t num, int32_t den) {
    if (den == 0) {
        return num < 0 ? FX_MIN : (num > 0 ? FX_MAX : 0);
    }
    return saturate(div_round((int64_t)num * FX_ONE, den));
}

int32_t fx_floor(fx_t v) {
    int32_t q = v / FX_ONE;
    if (v % FX_ONE < 0) {
        q--;
    }
    return q;
}

int32_t fx_round(fx_t v) {
    return (int32_t)div_round(v, FX_ONE);
}

fx_t fx_add(fx_t a, fx_t b) {
    return saturate((int64_t)a + b);
}

fx_t fx_sub(fx_t a, fx_t b) {
    return saturate((int64_t)a - b);
}

fx_t fx_mul(fx_t a, fx_t b) {
    return saturate(div_round((int64_t)a * b, FX_ONE));
}

fx_t fx_div(fx_t a, fx_t b) {
    if (b == 0) {
        return a < 0 ? FX_MIN : (a > 0 ? FX_MAX : 0);
    }
    return saturate(div_round((int64_t)a * FX_ONE, b));
}

fx_t fx_abs(fx_t v) {
    return v < 0 ? saturate(-(int64_t)v) : v;
}

/* Digit-by-digit integer square root of v << 16, so the result is already
 * in Q16.16. The radicand fits in 48 bits and the loop runs 24 times. */
fx_t fx_sqrt(fx_t v) {
    if (v <= 0) {
        return 0;
    }
    uint64_t n = (uint64_t)v << FX_SHIFT;
    uint64_t root = 0;
    uint64_t bit = (uint64_t)1 << 46;
    while (bit > n) {
        bit >>= 2;
    }
    while (bit != 0) {
        if (n >= root + bit) {
            n -= root + bit;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return (fx_t)root;
}
