#include "merge_sort.h"

#include <string.h>

static void merge(unsigned char *dst, const unsigned char *a, size_t na, const unsigned char *b,
                  size_t nb, size_t size, merge_sort_cmp cmp) {
    size_t i = 0, j = 0;
    while (i < na && j < nb) {
        if (cmp(b + j * size, a + i * size) < 0) {
            memcpy(dst, b + j * size, size);
            j++;
        } else {
            memcpy(dst, a + i * size, size);
            i++;
        }
        dst += size;
    }
    memcpy(dst, a + i * size, (na - i) * size);
    memcpy(dst + (na - i) * size, b + j * size, (nb - j) * size);
}

void merge_sort(void *base, size_t n, size_t size, merge_sort_cmp cmp, void *scratch) {
    unsigned char *a = base;
    unsigned char *tmp = scratch;
    for (size_t width = 1; width < n; width *= 2) {
        for (size_t lo = 0; lo < n; lo += 2 * width) {
            size_t mid = (lo + width < n) ? lo + width : n;
            size_t hi = (lo + 2 * width < n) ? lo + 2 * width : n;
            merge(tmp + lo * size, a + lo * size, mid - lo, a + mid * size, hi - mid, size, cmp);
        }
        memcpy(a, tmp, n * size);
    }
}
