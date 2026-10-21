#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct queue {
    uint8_t *data;
    size_t elem_size;
    size_t capacity;
    size_t head;
    size_t count;
} queue;

bool queue_init(queue *q, void *storage, size_t elem_size, size_t capacity);
size_t queue_count(const queue *q);
bool queue_is_empty(const queue *q);
bool queue_is_full(const queue *q);
bool queue_push(queue *q, const void *elem);
bool queue_pop(queue *q, void *out);
bool queue_peek(const queue *q, void *out);

#endif
