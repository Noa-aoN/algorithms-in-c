#include "crc32.h"

#define POLY UINT32_C(0xEDB88320)

static uint32_t table[256];
static int table_ready;

static void build_table(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int k = 0; k < 8; k++) {
            c = (c & 1) ? (c >> 1) ^ POLY : c >> 1;
        }
        table[i] = c;
    }
    table_ready = 1;
}

uint32_t crc32_update(uint32_t crc, const void *data, size_t len) {
    const uint8_t *p = data;
    if (!table_ready) {
        build_table();
    }
    for (size_t i = 0; i < len; i++) {
        crc = table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc;
}

uint32_t crc32_finish(uint32_t crc) { return crc ^ UINT32_C(0xFFFFFFFF); }

uint32_t crc32(const void *data, size_t len) {
    return crc32_finish(crc32_update(CRC32_INIT, data, len));
}

uint32_t crc32_bitwise(const void *data, size_t len) {
    const uint8_t *p = data;
    uint32_t crc = CRC32_INIT;
    for (size_t i = 0; i < len; i++) {
        crc ^= p[i];
        for (int k = 0; k < 8; k++) {
            crc = (crc & 1) ? (crc >> 1) ^ POLY : crc >> 1;
        }
    }
    return crc32_finish(crc);
}
