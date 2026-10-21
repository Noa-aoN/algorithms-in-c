#include "linked_list.h"

void list_init(list *l) {
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
}

size_t list_count(const list *l) { return l->count; }

bool list_is_empty(const list *l) { return l->head == NULL; }

void list_push_front(list *l, list_node *n) {
    n->next = l->head;
    l->head = n;
    if (l->tail == NULL) {
        l->tail = n;
    }
    l->count++;
}

void list_push_back(list *l, list_node *n) {
    n->next = NULL;
    if (l->tail != NULL) {
        l->tail->next = n;
    } else {
        l->head = n;
    }
    l->tail = n;
    l->count++;
}

list_node *list_pop_front(list *l) {
    list_node *n = l->head;
    if (n == NULL) {
        return NULL;
    }
    l->head = n->next;
    if (l->head == NULL) {
        l->tail = NULL;
    }
    n->next = NULL;
    l->count--;
    return n;
}

/* A pointer to the link, not to the node, so the head needs no special
 * case and the loop is the same for every position. */
bool list_remove(list *l, list_node *n) {
    list_node **link = &l->head;
    list_node *prev = NULL;
    while (*link != NULL && *link != n) {
        prev = *link;
        link = &(*link)->next;
    }
    if (*link == NULL) {
        return false;
    }
    *link = n->next;
    if (l->tail == n) {
        l->tail = prev;
    }
    n->next = NULL;
    l->count--;
    return true;
}

void list_reverse(list *l) {
    list_node *prev = NULL;
    list_node *cur = l->head;
    l->tail = cur;
    while (cur != NULL) {
        list_node *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    l->head = prev;
}
