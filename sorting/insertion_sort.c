#include "insertion_sort.h"

static void swap_bytes(unsigned char *a, unsigned char *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        unsigned char t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

void insertion_sort(void *base, size_t n, size_t size, insertion_sort_cmp cmp) {
    unsigned char *p = base;
    for (size_t i = 1; i < n; i++) {
        for (size_t j = i; j > 0 && cmp(p + (j - 1) * size, p + j * size) > 0; j--) {
            swap_bytes(p + (j - 1) * size, p + j * size, size);
        }
    }
}
