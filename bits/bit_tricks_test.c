#include "bit_tricks.h"

#include "minitest.h"

static unsigned ref_popcount(uint32_t v) {
    unsigned n = 0;
    for (; v != 0; v >>= 1) {
        n += v & 1u;
    }
    return n;
}

static unsigned ref_ctz(uint32_t v) {
    unsigned n = 0;
    if (v == 0) {
        return 32;
    }
    for (; (v & 1u) == 0; v >>= 1) {
        n++;
    }
    return n;
}

static unsigned ref_clz(uint32_t v) {
    unsigned n = 0;
    if (v == 0) {
        return 32;
    }
    for (; (v & 0x80000000u) == 0; v <<= 1) {
        n++;
    }
    return n;
}

static uint32_t ref_reverse(uint32_t v) {
    uint32_t r = 0;
    for (int i = 0; i < 32; i++) {
        r = (r << 1) | ((v >> i) & 1u);
    }
    return r;
}

static uint32_t next(uint32_t *s) {
    *s ^= *s << 13;
    *s ^= *s >> 17;
    *s ^= *s << 5;
    return *s;
}

static void test_pow2(void) {
    MT_CHECK(!bt_is_pow2(0));
    MT_CHECK(bt_is_pow2(1));
    MT_CHECK(bt_is_pow2(0x80000000u));
    MT_CHECK(!bt_is_pow2(6));
    MT_CHECK(bt_next_pow2(0) == 1);
    MT_CHECK(bt_next_pow2(1) == 1);
    MT_CHECK(bt_next_pow2(2) == 2);
    MT_CHECK(bt_next_pow2(3) == 4);
    MT_CHECK(bt_next_pow2(1000) == 1024);
    MT_CHECK(bt_next_pow2(0x80000000u) == 0x80000000u);
    MT_CHECK(bt_next_pow2(0x80000001u) == 0);
    MT_CHECK(bt_next_pow2(UINT32_MAX) == 0);
    for (unsigned i = 0; i < 32; i++) {
        uint32_t p = 1u << i;
        MT_CHECK(bt_is_pow2(p) && bt_next_pow2(p) == p);
        MT_CHECK(i == 0 || !bt_is_pow2(p + 1));
    }
}

static void test_counts_match_reference(void) {
    MT_CHECK(bt_popcount(0) == 0 && bt_ctz(0) == 32 && bt_clz(0) == 32);
    MT_CHECK(bt_popcount(UINT32_MAX) == 32 && bt_ctz(1) == 0 && bt_clz(1) == 31);
    uint32_t s = 99;
    for (int i = 0; i < 100000; i++) {
        uint32_t v = next(&s) >> (next(&s) % 32);
        MT_CHECK(bt_popcount(v) == ref_popcount(v));
        MT_CHECK(bt_ctz(v) == ref_ctz(v));
        MT_CHECK(bt_clz(v) == ref_clz(v));
        MT_CHECK(bt_parity(v) == ((ref_popcount(v) & 1u) != 0));
    }
}

static void test_reverse_and_bswap(void) {
    MT_CHECK(bt_bswap(0x11223344u) == 0x44332211u);
    MT_CHECK(bt_reverse(1) == 0x80000000u);
    MT_CHECK(bt_reverse(0xF0000000u) == 0x0000000Fu);
    uint32_t s = 5;
    for (int i = 0; i < 100000; i++) {
        uint32_t v = next(&s);
        MT_CHECK(bt_reverse(v) == ref_reverse(v));
        MT_CHECK(bt_reverse(bt_reverse(v)) == v);
        MT_CHECK(bt_bswap(bt_bswap(v)) == v);
    }
}

static void test_rotate(void) {
    MT_CHECK(bt_rotl(0x80000001u, 1) == 0x00000003u);
    MT_CHECK(bt_rotr(0x00000003u, 1) == 0x80000001u);
    MT_CHECK(bt_rotl(0x12345678u, 0) == 0x12345678u);
    MT_CHECK(bt_rotl(0x12345678u, 32) == 0x12345678u);
    MT_CHECK(bt_rotl(0x12345678u, 8) == 0x34567812u);
    MT_CHECK(bt_rotr(0x12345678u, 8) == 0x78123456u);
    uint32_t s = 11;
    for (int i = 0; i < 100000; i++) {
        uint32_t v = next(&s);
        unsigned n = next(&s) % 64;
        MT_CHECK(bt_rotr(bt_rotl(v, n), n) == v);
        MT_CHECK(bt_rotl(v, n) == bt_rotr(v, (32 - (n % 32)) % 32));
        MT_CHECK(bt_popcount(bt_rotl(v, n)) == bt_popcount(v));
    }
}

int main(void) {
    MT_RUN(test_pow2);
    MT_RUN(test_counts_match_reference);
    MT_RUN(test_reverse_and_bswap);
    MT_RUN(test_rotate);
    return mt_report("bit_tricks");
}
