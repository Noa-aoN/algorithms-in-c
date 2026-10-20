#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stddef.h>

typedef int (*quick_sort_cmp)(const void *a, const void *b);

void quick_sort(void *base, size_t n, size_t size, quick_sort_cmp cmp);

#endif
