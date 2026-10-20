#include "cobs.h"

/* Consistent Overhead Byte Stuffing: removes every 0x00 from a frame so that
 * 0x00 can be used as the frame delimiter on a byte stream (UART, SLIP-style
 * links). Each code byte says how many non-zero bytes follow before the next
 * zero; 0xFF means 254 bytes with no zero after them. Both functions return 0
 * on error (output too small, or malformed input for decode). */

size_t cobs_encode(const uint8_t *src, size_t len, uint8_t *dst, size_t cap) {
    size_t code_pos = 0, out = 1;
    uint8_t code = 1;
    if (cap == 0) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (src[i] == 0) {
            dst[code_pos] = code;
            code_pos = out++;
            code = 1;
        } else {
            if (out >= cap) {
                return 0;
            }
            dst[out++] = src[i];
            code++;
            if (code == 0xFF && i + 1 < len) {
                dst[code_pos] = code;
                code_pos = out++;
                code = 1;
            }
        }
        if (out > cap) {
            return 0;
        }
    }
    if (code_pos >= cap) {
        return 0;
    }
    dst[code_pos] = code;
    return out;
}

size_t cobs_decode(const uint8_t *src, size_t len, uint8_t *dst, size_t cap) {
    size_t in = 0, out = 0;
    while (in < len) {
        uint8_t code = src[in++];
        if (code == 0 || in + code - 1 > len) {
            return 0;
        }
        for (uint8_t i = 1; i < code; i++) {
            if (src[in] == 0 || out >= cap) {
                return 0;
            }
            dst[out++] = src[in++];
        }
        if (code != 0xFF && in < len) {
            if (out >= cap) {
                return 0;
            }
            dst[out++] = 0;
        }
    }
    return out;
}
