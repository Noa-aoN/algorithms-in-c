#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <stddef.h>

typedef int (*merge_sort_cmp)(const void *a, const void *b);

void merge_sort(void *base, size_t n, size_t size, merge_sort_cmp cmp, void *scratch);

#endif
