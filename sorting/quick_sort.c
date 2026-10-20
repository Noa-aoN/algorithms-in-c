#include "quick_sort.h"

static void swap_bytes(unsigned char *a, unsigned char *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        unsigned char t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

static void median_of_three(unsigned char *p, size_t n, size_t size, quick_sort_cmp cmp) {
    unsigned char *mid = p + (n / 2) * size;
    unsigned char *last = p + (n - 1) * size;
    if (cmp(mid, p) < 0) {
        swap_bytes(mid, p, size);
    }
    if (cmp(last, p) < 0) {
        swap_bytes(last, p, size);
    }
    if (cmp(last, mid) < 0) {
        swap_bytes(last, mid, size);
    }
    swap_bytes(p, mid, size);
}

static size_t partition(unsigned char *p, size_t n, size_t size, quick_sort_cmp cmp) {
    median_of_three(p, n, size, cmp);
    size_t i = 1, j = n - 1;
    for (;;) {
        while (i <= j && cmp(p + i * size, p) < 0) {
            i++;
        }
        while (cmp(p + j * size, p) > 0) {
            j--;
        }
        if (i >= j) {
            break;
        }
        swap_bytes(p + i * size, p + j * size, size);
        i++;
        j--;
    }
    swap_bytes(p, p + j * size, size);
    return j;
}

void quick_sort(void *base, size_t n, size_t size, quick_sort_cmp cmp) {
    unsigned char *p = base;
    while (n > 1) {
        size_t k = partition(p, n, size, cmp);
        size_t left = k, right = n - k - 1;
        if (left < right) {
            quick_sort(p, left, size, cmp);
            p += (k + 1) * size;
            n = right;
        } else {
            quick_sort(p + (k + 1) * size, right, size, cmp);
            n = left;
        }
    }
}
