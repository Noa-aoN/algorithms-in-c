#include "stack.h"

#include <string.h>

bool stack_init(stack *s, void *storage, size_t elem_size, size_t capacity) {
    if (storage == NULL || elem_size == 0 || capacity == 0) {
        return false;
    }
    s->data = storage;
    s->elem_size = elem_size;
    s->capacity = capacity;
    s->count = 0;
    return true;
}

size_t stack_count(const stack *s) { return s->count; }

bool stack_is_empty(const stack *s) { return s->count == 0; }

bool stack_is_full(const stack *s) { return s->count == s->capacity; }

bool stack_push(stack *s, const void *elem) {
    if (s->count == s->capacity) {
        return false;
    }
    memcpy(s->data + s->count * s->elem_size, elem, s->elem_size);
    s->count++;
    return true;
}

bool stack_pop(stack *s, void *out) {
    if (s->count == 0) {
        return false;
    }
    s->count--;
    memcpy(out, s->data + s->count * s->elem_size, s->elem_size);
    return true;
}

bool stack_peek(const stack *s, void *out) {
    if (s->count == 0) {
        return false;
    }
    memcpy(out, s->data + (s->count - 1) * s->elem_size, s->elem_size);
    return true;
}
