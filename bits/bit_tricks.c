#include "bit_tricks.h"

bool bt_is_pow2(uint32_t v) {
    return v != 0 && (v & (v - 1)) == 0;
}

/* Smallest power of two >= v. Returns 0 when it does not fit in 32 bits. */
uint32_t bt_next_pow2(uint32_t v) {
    if (v <= 1) {
        return 1;
    }
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v + 1;
}

unsigned bt_popcount(uint32_t v) {
    v = v - ((v >> 1) & 0x55555555u);
    v = (v & 0x33333333u) + ((v >> 2) & 0x33333333u);
    v = (v + (v >> 4)) & 0x0F0F0F0Fu;
    return (v * 0x01010101u) >> 24;
}

unsigned bt_ctz(uint32_t v) {
    if (v == 0) {
        return 32;
    }
    return bt_popcount((v & (0u - v)) - 1);
}

unsigned bt_clz(uint32_t v) {
    if (v == 0) {
        return 32;
    }
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return 32 - bt_popcount(v);
}

bool bt_parity(uint32_t v) {
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return (v & 1u) != 0;
}

uint32_t bt_reverse(uint32_t v) {
    v = ((v >> 1) & 0x55555555u) | ((v & 0x55555555u) << 1);
    v = ((v >> 2) & 0x33333333u) | ((v & 0x33333333u) << 2);
    v = ((v >> 4) & 0x0F0F0F0Fu) | ((v & 0x0F0F0F0Fu) << 4);
    return bt_bswap(v);
}

uint32_t bt_bswap(uint32_t v) {
    return (v >> 24) | ((v >> 8) & 0x0000FF00u) | ((v << 8) & 0x00FF0000u) | (v << 24);
}
