#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct stack {
    uint8_t *data;
    size_t elem_size;
    size_t capacity;
    size_t count;
} stack;

bool stack_init(stack *s, void *storage, size_t elem_size, size_t capacity);
size_t stack_count(const stack *s);
bool stack_is_empty(const stack *s);
bool stack_is_full(const stack *s);
bool stack_push(stack *s, const void *elem);
bool stack_pop(stack *s, void *out);
bool stack_peek(const stack *s, void *out);

#endif
