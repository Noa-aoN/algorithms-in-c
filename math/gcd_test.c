#include "gcd.h"

#include "minitest.h"

static uint32_t euclid(uint32_t a, uint32_t b) {
    while (b != 0) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static uint32_t next(uint32_t *s) {
    *s ^= *s << 13;
    *s ^= *s >> 17;
    *s ^= *s << 5;
    return *s;
}

static void test_gcd_known_values(void) {
    MT_CHECK(gcd_u32(0, 0) == 0);
    MT_CHECK(gcd_u32(0, 7) == 7);
    MT_CHECK(gcd_u32(7, 0) == 7);
    MT_CHECK(gcd_u32(12, 18) == 6);
    MT_CHECK(gcd_u32(17, 5) == 1);
    MT_CHECK(gcd_u32(1 << 20, 1 << 12) == 1 << 12);
    MT_CHECK(gcd_u32(UINT32_MAX, UINT32_MAX - 1) == 1);
    MT_CHECK(gcd_u32(UINT32_MAX, 0xFFFF) == 0xFFFF);
}

static void test_gcd_matches_euclid(void) {
    uint32_t s = 2026;
    for (int i = 0; i < 100000; i++) {
        uint32_t a = next(&s) >> (next(&s) % 32);
        uint32_t b = next(&s) >> (next(&s) % 32);
        MT_CHECK(gcd_u32(a, b) == euclid(a, b));
    }
}

static void test_lcm(void) {
    uint32_t l = 1;
    MT_CHECK(lcm_u32(4, 6, &l) && l == 12);
    MT_CHECK(lcm_u32(0, 6, &l) && l == 0);
    MT_CHECK(lcm_u32(7, 1, &l) && l == 7);
    MT_CHECK(lcm_u32(65536, 65535, &l) && l == 4294901760u);
    MT_CHECK(!lcm_u32(65536, 65537, &l));
    MT_CHECK(!lcm_u32(UINT32_MAX, UINT32_MAX - 1, &l));
}

static void test_extended_gcd_bezout(void) {
    int64_t x, y;
    MT_CHECK(gcd_ext_i64(240, 46, &x, &y) == 2 && 240 * x + 46 * y == 2);
    MT_CHECK(gcd_ext_i64(0, 5, &x, &y) == 5 && y == 1);
    MT_CHECK(gcd_ext_i64(-12, 18, &x, &y) == 6 && -12 * x + 18 * y == 6);
    MT_CHECK(gcd_ext_i64(12, -18, &x, &y) == 6 && 12 * x - 18 * y == 6);
    uint32_t s = 7;
    for (int i = 0; i < 10000; i++) {
        int64_t a = (int64_t)(next(&s) >> 2) - (1 << 29);
        int64_t b = (int64_t)(next(&s) >> 2) - (1 << 29);
        int64_t g = gcd_ext_i64(a, b, &x, &y);
        MT_CHECK(g >= 0);
        MT_CHECK(a * x + b * y == g);
        MT_CHECK(g == (int64_t)euclid((uint32_t)(a < 0 ? -a : a), (uint32_t)(b < 0 ? -b : b)));
    }
}

static void test_mod_inverse(void) {
    uint32_t inv = 0;
    MT_CHECK(mod_inverse_u32(3, 7, &inv) && inv == 5);
    MT_CHECK(mod_inverse_u32(10, 17, &inv) && inv == 12);
    MT_CHECK(mod_inverse_u32(1, 2, &inv) && inv == 1);
    MT_CHECK(!mod_inverse_u32(6, 9, &inv));
    MT_CHECK(!mod_inverse_u32(0, 7, &inv));
    MT_CHECK(!mod_inverse_u32(5, 1, &inv));
    MT_CHECK(mod_inverse_u32(123456789, 1000000007, &inv) &&
             (uint64_t)inv * 123456789u % 1000000007u == 1);
}

int main(void) {
    MT_RUN(test_gcd_known_values);
    MT_RUN(test_gcd_matches_euclid);
    MT_RUN(test_lcm);
    MT_RUN(test_extended_gcd_bezout);
    MT_RUN(test_mod_inverse);
    return mt_report("gcd");
}
