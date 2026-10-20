#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <stddef.h>

typedef int (*insertion_sort_cmp)(const void *a, const void *b);

void insertion_sort(void *base, size_t n, size_t size, insertion_sort_cmp cmp);

#endif
