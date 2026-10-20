#include "binary_search.h"

#include "minitest.h"

static int cmp_int(const void *key, const void *elem) {
    int k = *(const int *)key, e = *(const int *)elem;
    return (k > e) - (k < e);
}

static const int sorted[] = {1, 3, 3, 3, 7, 9, 12};
#define N (sizeof sorted / sizeof sorted[0])

static void test_find_present_and_absent(void) {
    int k;
    k = 7;
    MT_CHECK(binary_search_find(&k, sorted, N, sizeof(int), cmp_int) == 4);
    k = 1;
    MT_CHECK(binary_search_find(&k, sorted, N, sizeof(int), cmp_int) == 0);
    k = 12;
    MT_CHECK(binary_search_find(&k, sorted, N, sizeof(int), cmp_int) == 6);
    k = 3;
    MT_CHECK(binary_search_find(&k, sorted, N, sizeof(int), cmp_int) == 1);
    k = 4;
    MT_CHECK(binary_search_find(&k, sorted, N, sizeof(int), cmp_int) == -1);
    k = 0;
    MT_CHECK(binary_search_find(&k, sorted, N, sizeof(int), cmp_int) == -1);
    k = 100;
    MT_CHECK(binary_search_find(&k, sorted, N, sizeof(int), cmp_int) == -1);
}

static void test_bounds_on_duplicates(void) {
    int k = 3;
    MT_CHECK(binary_search_lower_bound(&k, sorted, N, sizeof(int), cmp_int) == 1);
    MT_CHECK(binary_search_upper_bound(&k, sorted, N, sizeof(int), cmp_int) == 4);
    k = 8;
    MT_CHECK(binary_search_lower_bound(&k, sorted, N, sizeof(int), cmp_int) == 5);
    MT_CHECK(binary_search_upper_bound(&k, sorted, N, sizeof(int), cmp_int) == 5);
    k = 0;
    MT_CHECK(binary_search_lower_bound(&k, sorted, N, sizeof(int), cmp_int) == 0);
    k = 100;
    MT_CHECK(binary_search_upper_bound(&k, sorted, N, sizeof(int), cmp_int) == N);
}

static void test_empty_array(void) {
    int k = 1;
    MT_CHECK(binary_search_find(&k, sorted, 0, sizeof(int), cmp_int) == -1);
    MT_CHECK(binary_search_lower_bound(&k, sorted, 0, sizeof(int), cmp_int) == 0);
}

int main(void) {
    MT_RUN(test_find_present_and_absent);
    MT_RUN(test_bounds_on_duplicates);
    MT_RUN(test_empty_array);
    return mt_report("binary_search");
}
