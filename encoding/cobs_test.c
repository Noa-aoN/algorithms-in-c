#include "cobs.h"

#include <string.h>

#include "minitest.h"

static void check_roundtrip(const uint8_t *msg, size_t len, const uint8_t *want_enc,
                            size_t want_len) {
    uint8_t enc[600], dec[600];
    size_t n = cobs_encode(msg, len, enc, sizeof enc);
    MT_CHECK(n == want_len);
    if (want_enc) {
        MT_CHECK(n == want_len && memcmp(enc, want_enc, want_len) == 0);
    }
    MT_CHECK(memchr(enc, 0, n) == NULL);
    size_t m = cobs_decode(enc, n, dec, sizeof dec);
    MT_CHECK(m == len);
    MT_CHECK(m == len && (len == 0 || memcmp(dec, msg, len) == 0));
}

static void test_wikipedia_examples(void) {
    static const uint8_t m1[] = {0x00};
    static const uint8_t e1[] = {0x01, 0x01};
    static const uint8_t m2[] = {0x00, 0x00};
    static const uint8_t e2[] = {0x01, 0x01, 0x01};
    static const uint8_t m3[] = {0x11, 0x22, 0x00, 0x33};
    static const uint8_t e3[] = {0x03, 0x11, 0x22, 0x02, 0x33};
    static const uint8_t m4[] = {0x11, 0x22, 0x33, 0x44};
    static const uint8_t e4[] = {0x05, 0x11, 0x22, 0x33, 0x44};
    static const uint8_t m5[] = {0x11, 0x00, 0x00, 0x00};
    static const uint8_t e5[] = {0x02, 0x11, 0x01, 0x01, 0x01};
    check_roundtrip(m1, 1, e1, 2);
    check_roundtrip(m2, 2, e2, 3);
    check_roundtrip(m3, 4, e3, 5);
    check_roundtrip(m4, 4, e4, 5);
    check_roundtrip(m5, 4, e5, 5);
}

static void test_empty_message(void) {
    static const uint8_t e[] = {0x01};
    check_roundtrip(NULL, 0, e, 1);
}

static void test_long_runs_without_zero(void) {
    uint8_t msg[512];
    for (size_t i = 0; i < sizeof msg; i++) {
        msg[i] = (uint8_t)((i % 255) + 1);
    }
    check_roundtrip(msg, 254, NULL, 255);
    check_roundtrip(msg, 255, NULL, 257);
    check_roundtrip(msg, 508, NULL, 510);
    check_roundtrip(msg, 512, NULL, 515);
    msg[254] = 0;
    check_roundtrip(msg, 255, NULL, 257);
    msg[253] = 0;
    check_roundtrip(msg, 254, NULL, 255);
}

static void test_output_capacity_is_respected(void) {
    static const uint8_t msg[] = {0x11, 0x22, 0x00, 0x33};
    uint8_t enc[5], dec[4];
    MT_CHECK(cobs_encode(msg, 4, enc, 4) == 0);
    MT_CHECK(cobs_encode(msg, 4, enc, 5) == 5);
    MT_CHECK(cobs_decode(enc, 5, dec, 3) == 0);
    MT_CHECK(cobs_decode(enc, 5, dec, 4) == 4);
}

static void test_malformed_input_is_rejected(void) {
    static const uint8_t zero_code[] = {0x00, 0x11};
    static const uint8_t truncated[] = {0x05, 0x11, 0x22};
    static const uint8_t embedded_zero[] = {0x03, 0x11, 0x00};
    uint8_t dec[8];
    MT_CHECK(cobs_decode(zero_code, 2, dec, sizeof dec) == 0);
    MT_CHECK(cobs_decode(truncated, 3, dec, sizeof dec) == 0);
    MT_CHECK(cobs_decode(embedded_zero, 3, dec, sizeof dec) == 0);
}

static void test_encode_max_bound(void) {
    MT_CHECK(COBS_ENCODE_MAX(0) == 1);
    MT_CHECK(COBS_ENCODE_MAX(254) == 255);
    MT_CHECK(COBS_ENCODE_MAX(255) == 257);
    MT_CHECK(COBS_ENCODE_MAX(508) == 510);
    MT_CHECK(COBS_ENCODE_MAX(512) == 515);
}

int main(void) {
    MT_RUN(test_wikipedia_examples);
    MT_RUN(test_empty_message);
    MT_RUN(test_long_runs_without_zero);
    MT_RUN(test_output_capacity_is_respected);
    MT_RUN(test_malformed_input_is_rejected);
    MT_RUN(test_encode_max_bound);
    return mt_report("cobs");
}
