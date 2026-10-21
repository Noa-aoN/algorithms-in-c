#include "fixed_point.h"

#include "minitest.h"

static void test_int_conversions(void) {
    MT_CHECK(fx_from_int(0) == 0);
    MT_CHECK(fx_from_int(1) == FX_ONE);
    MT_CHECK(fx_from_int(-3) == -3 * FX_ONE);
    MT_CHECK(fx_from_int(32767) == 32767 * FX_ONE);
    MT_CHECK(fx_from_int(32768) == FX_MAX);
    MT_CHECK(fx_from_int(-32768) == FX_MIN);
    MT_CHECK(fx_from_int(-40000) == FX_MIN);

    fx_t one_and_half = FX_ONE + FX_ONE / 2;
    MT_CHECK(fx_floor(one_and_half) == 1);
    MT_CHECK(fx_floor(-one_and_half) == -2);
    MT_CHECK(fx_floor(-1) == -1);
    MT_CHECK(fx_floor(FX_ONE) == 1);
    MT_CHECK(fx_round(one_and_half) == 2);
    MT_CHECK(fx_round(-one_and_half) == -2);
    MT_CHECK(fx_round(FX_ONE + FX_ONE / 4) == 1);
    MT_CHECK(fx_round(-(FX_ONE + FX_ONE / 4)) == -1);
    MT_CHECK(fx_round(FX_MAX) == 32768);
    MT_CHECK(fx_round(FX_MIN) == -32768);
}

static void test_from_ratio(void) {
    MT_CHECK(fx_from_ratio(1, 2) == FX_ONE / 2);
    MT_CHECK(fx_from_ratio(1, 3) == 21845);
    MT_CHECK(fx_from_ratio(2, 3) == 43691);
    MT_CHECK(fx_from_ratio(-1, 3) == -21845);
    MT_CHECK(fx_from_ratio(1, -3) == -21845);
    MT_CHECK(fx_from_ratio(-2, -3) == 43691);
    MT_CHECK(fx_from_ratio(100000, 1) == FX_MAX);
    MT_CHECK(fx_from_ratio(1, 0) == FX_MAX);
    MT_CHECK(fx_from_ratio(-1, 0) == FX_MIN);
    MT_CHECK(fx_from_ratio(0, 0) == 0);
}

static void test_add_sub_saturate(void) {
    MT_CHECK(fx_add(FX_ONE, FX_ONE / 2) == FX_ONE + FX_ONE / 2);
    MT_CHECK(fx_add(FX_MAX, 1) == FX_MAX);
    MT_CHECK(fx_add(FX_MIN, -1) == FX_MIN);
    MT_CHECK(fx_sub(FX_MIN, 1) == FX_MIN);
    MT_CHECK(fx_sub(FX_MAX, -1) == FX_MAX);
    MT_CHECK(fx_sub(0, FX_MIN) == FX_MAX);
    MT_CHECK(fx_abs(-FX_ONE) == FX_ONE);
    MT_CHECK(fx_abs(FX_MIN) == FX_MAX);
}

static void test_mul_known_values(void) {
    fx_t half = FX_ONE / 2;
    MT_CHECK(fx_mul(fx_from_int(3), fx_from_int(4)) == fx_from_int(12));
    MT_CHECK(fx_mul(fx_from_int(-3), fx_from_int(4)) == fx_from_int(-12));
    MT_CHECK(fx_mul(half, half) == FX_ONE / 4);
    MT_CHECK(fx_mul(FX_ONE + half, fx_from_int(2)) == fx_from_int(3));
    MT_CHECK(fx_mul(1, half) == 1);
    MT_CHECK(fx_mul(-1, half) == -1);
    MT_CHECK(fx_mul(1, FX_ONE / 4) == 0);
    MT_CHECK(fx_mul(fx_from_int(200), fx_from_int(200)) == FX_MAX);
    MT_CHECK(fx_mul(fx_from_int(-200), fx_from_int(200)) == FX_MIN);
    MT_CHECK(fx_mul(FX_MIN, -FX_ONE) == FX_MAX);
}

static void test_div_known_values(void) {
    MT_CHECK(fx_div(fx_from_int(12), fx_from_int(4)) == fx_from_int(3));
    MT_CHECK(fx_div(fx_from_int(1), fx_from_int(3)) == 21845);
    MT_CHECK(fx_div(fx_from_int(2), fx_from_int(3)) == 43691);
    MT_CHECK(fx_div(fx_from_int(-2), fx_from_int(3)) == -43691);
    MT_CHECK(fx_div(FX_ONE / 2, fx_from_int(2)) == FX_ONE / 4);
    MT_CHECK(fx_div(fx_from_int(1), 1) == FX_MAX);
    MT_CHECK(fx_div(fx_from_int(1), 0) == FX_MAX);
    MT_CHECK(fx_div(fx_from_int(-1), 0) == FX_MIN);
    MT_CHECK(fx_div(0, 0) == 0);
    MT_CHECK(fx_div(FX_MIN, -FX_ONE) == FX_MAX);
}

static uint32_t next(uint32_t *s) {
    *s ^= *s << 13;
    *s ^= *s >> 17;
    *s ^= *s << 5;
    return *s;
}

static void test_mul_div_against_exact_rationals(void) {
    uint32_t s = 2026;
    for (int i = 0; i < 20000; i++) {
        fx_t a = (fx_t)(next(&s) >> 8) - (1 << 23);
        fx_t b = (fx_t)(next(&s) >> 8) - (1 << 23);
        int64_t p = (int64_t)a * b;
        int64_t m = fx_mul(a, b);
        int64_t err = m * FX_ONE - p;
        MT_CHECK(err <= FX_ONE / 2 && err >= -FX_ONE / 2);
        if (b != 0 && (a >= 0 ? a : -a) < (b >= 0 ? b : -b) * 1024) {
            int64_t q = fx_div(a, b);
            int64_t rem = (int64_t)a * FX_ONE - q * b;
            int64_t ab = b < 0 ? -b : b;
            MT_CHECK(2 * (rem < 0 ? -rem : rem) <= ab);
        }
    }
}

static void test_sqrt(void) {
    MT_CHECK(fx_sqrt(0) == 0);
    MT_CHECK(fx_sqrt(-FX_ONE) == 0);
    MT_CHECK(fx_sqrt(FX_ONE) == FX_ONE);
    MT_CHECK(fx_sqrt(fx_from_int(4)) == fx_from_int(2));
    MT_CHECK(fx_sqrt(fx_from_int(144)) == fx_from_int(12));
    MT_CHECK(fx_sqrt(FX_ONE / 4) == FX_ONE / 2);
    MT_CHECK(fx_sqrt(fx_from_int(2)) == 92681);
    MT_CHECK(fx_sqrt(FX_MAX) == 11863283);
    for (int32_t i = 0; i <= 32767; i += 7) {
        fx_t r = fx_sqrt(fx_from_int(i));
        int64_t sq = (int64_t)r * r;
        int64_t target = (int64_t)i * FX_ONE * FX_ONE;
        MT_CHECK(sq <= target && (int64_t)(r + 1) * (r + 1) > target);
    }
}

int main(void) {
    MT_RUN(test_int_conversions);
    MT_RUN(test_from_ratio);
    MT_RUN(test_add_sub_saturate);
    MT_RUN(test_mul_known_values);
    MT_RUN(test_div_known_values);
    MT_RUN(test_mul_div_against_exact_rationals);
    MT_RUN(test_sqrt);
    return mt_report("fixed_point");
}
