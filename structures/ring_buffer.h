#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ring_buffer {
    uint8_t *data;
    size_t mask;
    size_t head;
    size_t tail;
} ring_buffer;

bool ring_buffer_init(ring_buffer *rb, uint8_t *storage, size_t capacity);
size_t ring_buffer_capacity(const ring_buffer *rb);
size_t ring_buffer_count(const ring_buffer *rb);
bool ring_buffer_is_empty(const ring_buffer *rb);
bool ring_buffer_is_full(const ring_buffer *rb);
bool ring_buffer_push(ring_buffer *rb, uint8_t byte);
bool ring_buffer_pop(ring_buffer *rb, uint8_t *out);
size_t ring_buffer_write(ring_buffer *rb, const uint8_t *src, size_t n);
size_t ring_buffer_read(ring_buffer *rb, uint8_t *dst, size_t n);

#endif
