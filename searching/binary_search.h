#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <stddef.h>

typedef int (*binary_search_cmp)(const void *key, const void *elem);

size_t binary_search_lower_bound(const void *key, const void *base, size_t n, size_t size,
                                 binary_search_cmp cmp);
size_t binary_search_upper_bound(const void *key, const void *base, size_t n, size_t size,
                                 binary_search_cmp cmp);
long binary_search_find(const void *key, const void *base, size_t n, size_t size,
                        binary_search_cmp cmp);

#endif
