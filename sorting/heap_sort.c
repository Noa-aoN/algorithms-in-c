#include "heap_sort.h"

static void swap_bytes(unsigned char *a, unsigned char *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        unsigned char t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

static void sift_down(unsigned char *p, size_t root, size_t n, size_t size, heap_sort_cmp cmp) {
    for (;;) {
        size_t child = 2 * root + 1;
        if (child >= n) {
            return;
        }
        if (child + 1 < n && cmp(p + child * size, p + (child + 1) * size) < 0) {
            child++;
        }
        if (cmp(p + root * size, p + child * size) >= 0) {
            return;
        }
        swap_bytes(p + root * size, p + child * size, size);
        root = child;
    }
}

void heap_sort(void *base, size_t n, size_t size, heap_sort_cmp cmp) {
    unsigned char *p = base;
    if (n < 2) {
        return;
    }
    for (size_t i = n / 2; i-- > 0;) {
        sift_down(p, i, n, size, cmp);
    }
    for (size_t end = n - 1; end > 0; end--) {
        swap_bytes(p, p + end * size, size);
        sift_down(p, 0, end, size, cmp);
    }
}
