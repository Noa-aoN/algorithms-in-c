#ifndef GCD_H
#define GCD_H

#include <stdbool.h>
#include <stdint.h>

uint32_t gcd_u32(uint32_t a, uint32_t b);
bool lcm_u32(uint32_t a, uint32_t b, uint32_t *out);
int64_t gcd_ext_i64(int64_t a, int64_t b, int64_t *x, int64_t *y);
bool mod_inverse_u32(uint32_t a, uint32_t m, uint32_t *out);

#endif
