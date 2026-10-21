#ifndef BIT_TRICKS_H
#define BIT_TRICKS_H

#include <stdbool.h>
#include <stdint.h>

/* Portable bit operations in plain C: no compiler builtins, so they behave
 * the same on every toolchain and serve as a reference for the intrinsics. */
bool bt_is_pow2(uint32_t v);
uint32_t bt_next_pow2(uint32_t v);
unsigned bt_popcount(uint32_t v);
unsigned bt_ctz(uint32_t v);
unsigned bt_clz(uint32_t v);
bool bt_parity(uint32_t v);
uint32_t bt_reverse(uint32_t v);
uint32_t bt_bswap(uint32_t v);

#endif
