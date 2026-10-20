#include "binary_search.h"

size_t binary_search_lower_bound(const void *key, const void *base, size_t n, size_t size,
                                 binary_search_cmp cmp) {
    const unsigned char *p = base;
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (cmp(key, p + mid * size) > 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

size_t binary_search_upper_bound(const void *key, const void *base, size_t n, size_t size,
                                 binary_search_cmp cmp) {
    const unsigned char *p = base;
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (cmp(key, p + mid * size) >= 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

long binary_search_find(const void *key, const void *base, size_t n, size_t size,
                        binary_search_cmp cmp) {
    size_t i = binary_search_lower_bound(key, base, n, size, cmp);
    const unsigned char *p = base;
    if (i < n && cmp(key, p + i * size) == 0) {
        return (long)i;
    }
    return -1;
}
