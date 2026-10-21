#include "gcd.h"

/* Stein's binary GCD: shifts and subtractions only, so it costs the same on
 * a core without a hardware divider. */
uint32_t gcd_u32(uint32_t a, uint32_t b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    unsigned shift = 0;
    while (((a | b) & 1u) == 0) {
        a >>= 1;
        b >>= 1;
        shift++;
    }
    while ((a & 1u) == 0) {
        a >>= 1;
    }
    while (b != 0) {
        while ((b & 1u) == 0) {
            b >>= 1;
        }
        if (a > b) {
            uint32_t t = a;
            a = b;
            b = t;
        }
        b -= a;
    }
    return a << shift;
}

bool lcm_u32(uint32_t a, uint32_t b, uint32_t *out) {
    if (a == 0 || b == 0) {
        *out = 0;
        return true;
    }
    uint64_t l = (uint64_t)(a / gcd_u32(a, b)) * b;
    if (l > UINT32_MAX) {
        return false;
    }
    *out = (uint32_t)l;
    return true;
}

int64_t gcd_ext_i64(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    int64_t old_r = a, r = b;
    int64_t old_x = 1, cur_x = 0;
    int64_t old_y = 0, cur_y = 1;
    while (r != 0) {
        int64_t q = old_r / r;
        int64_t t;
        t = old_r - q * r;
        old_r = r;
        r = t;
        t = old_x - q * cur_x;
        old_x = cur_x;
        cur_x = t;
        t = old_y - q * cur_y;
        old_y = cur_y;
        cur_y = t;
    }
    if (old_r < 0) {
        old_r = -old_r;
        old_x = -old_x;
        old_y = -old_y;
    }
    *x = old_x;
    *y = old_y;
    return old_r;
}

bool mod_inverse_u32(uint32_t a, uint32_t m, uint32_t *out) {
    int64_t x, y;
    if (m < 2 || gcd_ext_i64((int64_t)a, (int64_t)m, &x, &y) != 1) {
        return false;
    }
    int64_t inv = x % (int64_t)m;
    if (inv < 0) {
        inv += (int64_t)m;
    }
    *out = (uint32_t)inv;
    return true;
}
