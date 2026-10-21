#include "linked_list.h"

#include "minitest.h"

typedef struct item {
    int value;
    list_node node;
} item;

static int values(const list *l, int *out, int cap) {
    int n = 0;
    for (list_node *p = l->head; p != NULL && n < cap; p = p->next) {
        out[n++] = LIST_ENTRY(p, item, node)->value;
    }
    return n;
}

static bool same(const list *l, const int *want, int n) {
    int got[16];
    if (values(l, got, 16) != n || (int)list_count(l) != n) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        if (got[i] != want[i]) {
            return false;
        }
    }
    if (n == 0) {
        return l->head == NULL && l->tail == NULL;
    }
    return LIST_ENTRY(l->tail, item, node)->value == want[n - 1] && l->tail->next == NULL;
}

static void test_push_front_and_back(void) {
    item a = {1, {NULL}}, b = {2, {NULL}}, c = {3, {NULL}};
    list l;
    list_init(&l);
    MT_CHECK(list_is_empty(&l) && same(&l, NULL, 0));
    list_push_back(&l, &b.node);
    list_push_front(&l, &a.node);
    list_push_back(&l, &c.node);
    MT_CHECK(same(&l, (int[]){1, 2, 3}, 3));
    MT_CHECK(!list_is_empty(&l));
}

static void test_pop_front_until_empty(void) {
    item a = {1, {NULL}}, b = {2, {NULL}};
    list l;
    list_init(&l);
    MT_CHECK(list_pop_front(&l) == NULL);
    list_push_back(&l, &a.node);
    list_push_back(&l, &b.node);
    MT_CHECK(list_pop_front(&l) == &a.node && a.node.next == NULL);
    MT_CHECK(same(&l, (int[]){2}, 1));
    MT_CHECK(list_pop_front(&l) == &b.node);
    MT_CHECK(same(&l, NULL, 0));
    list_push_back(&l, &a.node);
    MT_CHECK(same(&l, (int[]){1}, 1));
}

static void test_remove_head_middle_tail_and_missing(void) {
    item a = {1, {NULL}}, b = {2, {NULL}}, c = {3, {NULL}}, d = {4, {NULL}}, x = {9, {NULL}};
    list l;
    list_init(&l);
    list_push_back(&l, &a.node);
    list_push_back(&l, &b.node);
    list_push_back(&l, &c.node);
    list_push_back(&l, &d.node);
    MT_CHECK(!list_remove(&l, &x.node));
    MT_CHECK(list_remove(&l, &b.node) && same(&l, (int[]){1, 3, 4}, 3));
    MT_CHECK(list_remove(&l, &d.node) && same(&l, (int[]){1, 3}, 2));
    MT_CHECK(list_remove(&l, &a.node) && same(&l, (int[]){3}, 1));
    MT_CHECK(!list_remove(&l, &a.node));
    MT_CHECK(list_remove(&l, &c.node) && same(&l, NULL, 0));
    list_push_back(&l, &d.node);
    MT_CHECK(same(&l, (int[]){4}, 1));
}

static void test_reverse(void) {
    item a = {1, {NULL}}, b = {2, {NULL}}, c = {3, {NULL}};
    list l;
    list_init(&l);
    list_reverse(&l);
    MT_CHECK(same(&l, NULL, 0));
    list_push_back(&l, &a.node);
    list_reverse(&l);
    MT_CHECK(same(&l, (int[]){1}, 1));
    list_push_back(&l, &b.node);
    list_push_back(&l, &c.node);
    list_reverse(&l);
    MT_CHECK(same(&l, (int[]){3, 2, 1}, 3));
    list_push_back(&l, &(item){0, {NULL}}.node);
    MT_CHECK(list_count(&l) == 4);
}

int main(void) {
    MT_RUN(test_push_front_and_back);
    MT_RUN(test_pop_front_until_empty);
    MT_RUN(test_remove_head_middle_tail_and_missing);
    MT_RUN(test_reverse);
    return mt_report("linked_list");
}
