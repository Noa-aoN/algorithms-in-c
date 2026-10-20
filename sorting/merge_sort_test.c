#include "merge_sort.h"

#include <string.h>

#include "minitest.h"

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

typedef struct {
    int key;
    char tag;
} rec;

static int cmp_rec(const void *a, const void *b) {
    return ((const rec *)a)->key - ((const rec *)b)->key;
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
    int tmp[9];
    merge_sort(a, 9, sizeof a[0], cmp_int, tmp);
    MT_CHECK(memcmp(a, want, sizeof a) == 0);
}

static void test_empty_single_and_sorted(void) {
    int one[] = {42};
    int asc[] = {1, 2, 3, 4};
    int tmp[4];
    merge_sort(NULL, 0, sizeof(int), cmp_int, tmp);
    merge_sort(one, 1, sizeof one[0], cmp_int, tmp);
    merge_sort(asc, 4, sizeof asc[0], cmp_int, tmp);
    MT_CHECK(one[0] == 42);
    MT_CHECK(asc[0] == 1 && asc[3] == 4);
}

static void test_is_stable(void) {
    rec r[] = {{2, 'a'}, {1, 'b'}, {2, 'c'}, {1, 'd'}, {0, 'e'}, {2, 'f'}};
    rec tmp[6];
    merge_sort(r, 6, sizeof r[0], cmp_rec, tmp);
    MT_CHECK(r[0].tag == 'e');
    MT_CHECK(r[1].tag == 'b' && r[2].tag == 'd');
    MT_CHECK(r[3].tag == 'a' && r[4].tag == 'c' && r[5].tag == 'f');
}

static void test_random_arrays_are_sorted(void) {
    enum { N = 1000 };
    static int a[N], tmp[N];
    uint32_t s = 2463534242u;
    for (int round = 0; round < 5; round++) {
        size_t n = (size_t)(next(&s) % N) + 1;
        for (size_t i = 0; i < n; i++) {
            a[i] = (int)(next(&s) % 100) - 50;
        }
        merge_sort(a, n, sizeof a[0], cmp_int, tmp);
        for (size_t i = 1; i < n; i++) {
            MT_CHECK(a[i - 1] <= a[i]);
        }
    }
}

int main(void) {
    MT_RUN(test_sorts_ints);
    MT_RUN(test_empty_single_and_sorted);
    MT_RUN(test_is_stable);
    MT_RUN(test_random_arrays_are_sorted);
    return mt_report("merge_sort");
}
