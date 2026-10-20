#include "heap_sort.h"

#include <string.h>

#include "minitest.h"

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static uint32_t next(uint32_t *s) {
    *s ^= *s << 13;
    *s ^= *s >> 17;
    *s ^= *s << 5;
    return *s;
}

static void test_sorts_ints(void) {
    int a[] = {5, 2, 9, 1, 5, 6, -3, 0, 7};
    int want[] = {-3, 0, 1, 2, 5, 5, 6, 7, 9};
    heap_sort(a, 9, sizeof a[0], cmp_int);
    MT_CHECK(memcmp(a, want, sizeof a) == 0);
}

static void test_empty_single_two(void) {
    int one[] = {42};
    int two[] = {2, 1};
    heap_sort(NULL, 0, sizeof(int), cmp_int);
    heap_sort(one, 1, sizeof one[0], cmp_int);
    heap_sort(two, 2, sizeof two[0], cmp_int);
    MT_CHECK(one[0] == 42);
    MT_CHECK(two[0] == 1 && two[1] == 2);
}

static void test_degenerate_inputs(void) {
    enum { N = 2000 };
    static int a[N];
    for (int i = 0; i < N; i++) {
        a[i] = N - i;
    }
    heap_sort(a, N, sizeof a[0], cmp_int);
    for (int i = 1; i < N; i++) {
        MT_CHECK(a[i - 1] <= a[i]);
    }
    for (int i = 0; i < N; i++) {
        a[i] = 7;
    }
    heap_sort(a, N, sizeof a[0], cmp_int);
    MT_CHECK(a[0] == 7 && a[N - 1] == 7);
}

static void test_random_arrays_are_sorted(void) {
    enum { N = 1000 };
    static int a[N];
    uint32_t s = 123456789u;
    for (int round = 0; round < 5; round++) {
        size_t n = (size_t)(next(&s) % N) + 1;
        for (size_t i = 0; i < n; i++) {
            a[i] = (int)(next(&s) % 100) - 50;
        }
        heap_sort(a, n, sizeof a[0], cmp_int);
        for (size_t i = 1; i < n; i++) {
            MT_CHECK(a[i - 1] <= a[i]);
        }
    }
}

int main(void) {
    MT_RUN(test_sorts_ints);
    MT_RUN(test_empty_single_two);
    MT_RUN(test_degenerate_inputs);
    MT_RUN(test_random_arrays_are_sorted);
    return mt_report("heap_sort");
}
