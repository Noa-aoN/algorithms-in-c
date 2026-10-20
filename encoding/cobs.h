#ifndef COBS_H
#define COBS_H

#include <stddef.h>
#include <stdint.h>

#define COBS_ENCODE_MAX(n) ((n) == 0 ? 1 : (n) + ((n) + 253) / 254)

size_t cobs_encode(const uint8_t *src, size_t len, uint8_t *dst, size_t cap);
size_t cobs_decode(const uint8_t *src, size_t len, uint8_t *dst, size_t cap);

#endif
