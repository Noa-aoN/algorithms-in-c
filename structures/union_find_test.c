#include "union_find.h"

#include "minitest.h"

#define N 64

static void test_init_and_singletons(void) {
    uint32_t parent[N], size[N];
    union_find uf;
    MT_CHECK(!union_find_init(&uf, NULL, size, N));
    MT_CHECK(!union_find_init(&uf, parent, size, 0));
    MT_CHECK(union_find_init(&uf, parent, size, N));
    MT_CHECK(union_find_sets(&uf) == N);
    for (uint32_t i = 0; i < N; i++) {
        MT_CHECK(union_find_find(&uf, i) == i);
        MT_CHECK(union_find_size(&uf, i) == 1);
    }
    MT_CHECK(!union_find_same(&uf, 0, 1));
}

static void test_union_merges_and_counts(void) {
    uint32_t parent[N], size[N];
    union_find uf;
    union_find_init(&uf, parent, size, N);
    MT_CHECK(union_find_union(&uf, 1, 2));
    MT_CHECK(union_find_union(&uf, 3, 4));
    MT_CHECK(!union_find_union(&uf, 2, 1));
    MT_CHECK(union_find_sets(&uf) == N - 2);
    MT_CHECK(union_find_same(&uf, 1, 2) && !union_find_same(&uf, 2, 3));
    MT_CHECK(union_find_union(&uf, 2, 4));
    MT_CHECK(union_find_same(&uf, 1, 3));
    MT_CHECK(union_find_size(&uf, 4) == 4);
    MT_CHECK(union_find_sets(&uf) == N - 3);
}

static void test_chain_stays_flat(void) {
    uint32_t parent[N], size[N];
    union_find uf;
    union_find_init(&uf, parent, size, N);
    for (uint32_t i = 1; i < N; i++) {
        union_find_union(&uf, i - 1, i);
    }
    MT_CHECK(union_find_sets(&uf) == 1);
    MT_CHECK(union_find_size(&uf, 17) == N);
    uint32_t root = union_find_find(&uf, 0);
    for (uint32_t i = 0; i < N; i++) {
        MT_CHECK(union_find_find(&uf, i) == root);
        MT_CHECK(parent[i] == root || parent[parent[i]] == root);
    }
}

static void test_matches_naive_labels(void) {
    uint32_t parent[N], size[N], label[N];
    union_find uf;
    union_find_init(&uf, parent, size, N);
    for (uint32_t i = 0; i < N; i++) {
        label[i] = i;
    }
    uint32_t s = 42;
    for (int step = 0; step < 200; step++) {
        s ^= s << 13;
        s ^= s >> 17;
        s ^= s << 5;
        uint32_t a = s % N, b = (s / N) % N;
        bool merged = union_find_union(&uf, a, b);
        MT_CHECK(merged == (label[a] != label[b]));
        uint32_t from = label[b], to = label[a];
        for (uint32_t i = 0; i < N; i++) {
            if (label[i] == from) {
                label[i] = to;
            }
        }
        for (uint32_t i = 0; i < N; i++) {
            MT_CHECK(union_find_same(&uf, i, a) == (label[i] == label[a]));
        }
    }
}

int main(void) {
    MT_RUN(test_init_and_singletons);
    MT_RUN(test_union_merges_and_counts);
    MT_RUN(test_chain_stays_flat);
    MT_RUN(test_matches_naive_labels);
    return mt_report("union_find");
}
