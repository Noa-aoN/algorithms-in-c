#include "union_find.h"

#include <stddef.h>

bool union_find_init(union_find *uf, uint32_t *parent, uint32_t *size, uint32_t n) {
    if (parent == NULL || size == NULL || n == 0) {
        return false;
    }
    for (uint32_t i = 0; i < n; i++) {
        parent[i] = i;
        size[i] = 1;
    }
    uf->parent = parent;
    uf->size = size;
    uf->n = n;
    uf->sets = n;
    return true;
}

/* Path halving: one pass, no recursion and no second loop, with the same
 * amortised bound as full path compression. */
uint32_t union_find_find(union_find *uf, uint32_t x) {
    while (uf->parent[x] != x) {
        uf->parent[x] = uf->parent[uf->parent[x]];
        x = uf->parent[x];
    }
    return x;
}

bool union_find_union(union_find *uf, uint32_t a, uint32_t b) {
    uint32_t ra = union_find_find(uf, a);
    uint32_t rb = union_find_find(uf, b);
    if (ra == rb) {
        return false;
    }
    if (uf->size[ra] < uf->size[rb]) {
        uint32_t t = ra;
        ra = rb;
        rb = t;
    }
    uf->parent[rb] = ra;
    uf->size[ra] += uf->size[rb];
    uf->sets--;
    return true;
}

bool union_find_same(union_find *uf, uint32_t a, uint32_t b) {
    return union_find_find(uf, a) == union_find_find(uf, b);
}

uint32_t union_find_size(union_find *uf, uint32_t x) { return uf->size[union_find_find(uf, x)]; }

uint32_t union_find_sets(const union_find *uf) { return uf->sets; }
