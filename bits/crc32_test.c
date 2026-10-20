#include "crc32.h"

#include <string.h>

#include "minitest.h"

static void test_check_value(void) {
    MT_CHECK(crc32("123456789", 9) == UINT32_C(0xCBF43926));
    MT_CHECK(crc32_bitwise("123456789", 9) == UINT32_C(0xCBF43926));
}

static void test_empty_and_known_strings(void) {
    MT_CHECK(crc32("", 0) == 0);
    MT_CHECK(crc32("a", 1) == UINT32_C(0xE8B7BE43));
    MT_CHECK(crc32("abc", 3) == UINT32_C(0x352441C2));
    MT_CHECK(crc32("The quick brown fox jumps over the lazy dog", 43) == UINT32_C(0x414FA339));
}

static void test_incremental_matches_one_shot(void) {
    const char *s = "The quick brown fox jumps over the lazy dog";
    size_t len = strlen(s);
    for (size_t split = 0; split <= len; split++) {
        uint32_t crc = crc32_update(CRC32_INIT, s, split);
        crc = crc32_update(crc, s + split, len - split);
        MT_CHECK(crc32_finish(crc) == crc32(s, len));
    }
}

static void test_table_matches_bitwise_on_binary_data(void) {
    uint8_t buf[256];
    for (int i = 0; i < 256; i++) {
        buf[i] = (uint8_t)(i * 7 + 3);
    }
    for (size_t len = 0; len <= 256; len += 17) {
        MT_CHECK(crc32(buf, len) == crc32_bitwise(buf, len));
    }
}

static void test_detects_single_bit_flip(void) {
    uint8_t buf[64];
    memset(buf, 0xA5, sizeof buf);
    uint32_t base = crc32(buf, sizeof buf);
    for (size_t i = 0; i < sizeof buf * 8; i += 37) {
        buf[i / 8] ^= (uint8_t)(1u << (i % 8));
        MT_CHECK(crc32(buf, sizeof buf) != base);
        buf[i / 8] ^= (uint8_t)(1u << (i % 8));
    }
}

int main(void) {
    MT_RUN(test_check_value);
    MT_RUN(test_empty_and_known_strings);
    MT_RUN(test_incremental_matches_one_shot);
    MT_RUN(test_table_matches_bitwise_on_binary_data);
    MT_RUN(test_detects_single_bit_flip);
    return mt_report("crc32");
}
