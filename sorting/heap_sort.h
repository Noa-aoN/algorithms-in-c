#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <stddef.h>

typedef int (*heap_sort_cmp)(const void *a, const void *b);

void heap_sort(void *base, size_t n, size_t size, heap_sort_cmp cmp);

#endif
