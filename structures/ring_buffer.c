#include "ring_buffer.h"

bool ring_buffer_init(ring_buffer *rb, uint8_t *storage, size_t capacity) {
    if (storage == NULL || capacity == 0 || (capacity & (capacity - 1)) != 0) {
        return false;
    }
    rb->data = storage;
    rb->mask = capacity - 1;
    rb->head = 0;
    rb->tail = 0;
    return true;
}

size_t ring_buffer_capacity(const ring_buffer *rb) { return rb->mask + 1; }

size_t ring_buffer_count(const ring_buffer *rb) { return rb->head - rb->tail; }

bool ring_buffer_is_empty(const ring_buffer *rb) { return rb->head == rb->tail; }

bool ring_buffer_is_full(const ring_buffer *rb) { return ring_buffer_count(rb) > rb->mask; }

bool ring_buffer_push(ring_buffer *rb, uint8_t byte) {
    if (ring_buffer_is_full(rb)) {
        return false;
    }
    rb->data[rb->head & rb->mask] = byte;
    rb->head++;
    return true;
}

bool ring_buffer_pop(ring_buffer *rb, uint8_t *out) {
    if (ring_buffer_is_empty(rb)) {
        return false;
    }
    *out = rb->data[rb->tail & rb->mask];
    rb->tail++;
    return true;
}

size_t ring_buffer_write(ring_buffer *rb, const uint8_t *src, size_t n) {
    size_t i = 0;
    while (i < n && ring_buffer_push(rb, src[i])) {
        i++;
    }
    return i;
}

size_t ring_buffer_read(ring_buffer *rb, uint8_t *dst, size_t n) {
    size_t i = 0;
    while (i < n && ring_buffer_pop(rb, &dst[i])) {
        i++;
    }
    return i;
}
