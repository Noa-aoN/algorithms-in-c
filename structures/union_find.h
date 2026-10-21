#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stdbool.h>
#include <stdint.h>

typedef struct union_find {
    uint32_t *parent;
    uint32_t *size;
    uint32_t n;
    uint32_t sets;
} union_find;

bool union_find_init(union_find *uf, uint32_t *parent, uint32_t *size, uint32_t n);
uint32_t union_find_find(union_find *uf, uint32_t x);
bool union_find_union(union_find *uf, uint32_t a, uint32_t b);
bool union_find_same(union_find *uf, uint32_t a, uint32_t b);
uint32_t union_find_size(union_find *uf, uint32_t x);
uint32_t union_find_sets(const union_find *uf);

#endif
