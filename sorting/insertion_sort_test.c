#include "insertion_sort.h"

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

static void test_sorts_ints(void) {
    int a[] = {5, 2, 9, 1, 5, 6, -3, 0};
    int want[] = {-3, 0, 1, 2, 5, 5, 6, 9};
    insertion_sort(a, 8, sizeof a[0], cmp_int);
    MT_CHECK(memcmp(a, want, sizeof a) == 0);
}

static void test_handles_empty_and_single(void) {
    int one[] = {42};
    insertion_sort(NULL, 0, sizeof(int), cmp_int);
    insertion_sort(one, 1, sizeof one[0], cmp_int);
    MT_CHECK(one[0] == 42);
}

static void test_already_sorted_and_reversed(void) {
    int asc[] = {1, 2, 3, 4, 5};
    int desc[] = {5, 4, 3, 2, 1};
    int want[] = {1, 2, 3, 4, 5};
    insertion_sort(asc, 5, sizeof asc[0], cmp_int);
    insertion_sort(desc, 5, sizeof desc[0], cmp_int);
    MT_CHECK(memcmp(asc, want, sizeof asc) == 0);
    MT_CHECK(memcmp(desc, want, sizeof desc) == 0);
}

static void test_is_stable(void) {
    rec r[] = {{2, 'a'}, {1, 'b'}, {2, 'c'}, {1, 'd'}, {0, 'e'}};
    insertion_sort(r, 5, sizeof r[0], cmp_rec);
    MT_CHECK(r[0].tag == 'e');
    MT_CHECK(r[1].tag == 'b' && r[2].tag == 'd');
    MT_CHECK(r[3].tag == 'a' && r[4].tag == 'c');
}

int main(void) {
    MT_RUN(test_sorts_ints);
    MT_RUN(test_handles_empty_and_single);
    MT_RUN(test_already_sorted_and_reversed);
    MT_RUN(test_is_stable);
    return mt_report("insertion_sort");
}
