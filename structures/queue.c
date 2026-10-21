#include "queue.h"

#include <string.h>

bool queue_init(queue *q, void *storage, size_t elem_size, size_t capacity) {
    if (storage == NULL || elem_size == 0 || capacity == 0) {
        return false;
    }
    q->data = storage;
    q->elem_size = elem_size;
    q->capacity = capacity;
    q->head = 0;
    q->count = 0;
    return true;
}

size_t queue_count(const queue *q) { return q->count; }

bool queue_is_empty(const queue *q) { return q->count == 0; }

bool queue_is_full(const queue *q) { return q->count == q->capacity; }

/* head + count tracks the tail, so any capacity works and no slot is wasted;
 * the ring_buffer item uses free-running counters instead because it wants
 * a lock-free producer/consumer split, which this one does not promise. */
static size_t slot(const queue *q, size_t offset) {
    size_t i = q->head + offset;
    if (i >= q->capacity) {
        i -= q->capacity;
    }
    return i;
}

bool queue_push(queue *q, const void *elem) {
    if (q->count == q->capacity) {
        return false;
    }
    memcpy(q->data + slot(q, q->count) * q->elem_size, elem, q->elem_size);
    q->count++;
    return true;
}

bool queue_pop(queue *q, void *out) {
    if (q->count == 0) {
        return false;
    }
    memcpy(out, q->data + q->head * q->elem_size, q->elem_size);
    q->head = slot(q, 1);
    q->count--;
    return true;
}

bool queue_peek(const queue *q, void *out) {
    if (q->count == 0) {
        return false;
    }
    memcpy(out, q->data + q->head * q->elem_size, q->elem_size);
    return true;
}
