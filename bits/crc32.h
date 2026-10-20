#ifndef CRC32_H
#define CRC32_H

#include <stddef.h>
#include <stdint.h>

#define CRC32_INIT UINT32_C(0xFFFFFFFF)

uint32_t crc32_update(uint32_t crc, const void *data, size_t len);
uint32_t crc32_finish(uint32_t crc);
uint32_t crc32(const void *data, size_t len);
uint32_t crc32_bitwise(const void *data, size_t len);

#endif
