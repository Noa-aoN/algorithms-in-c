#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

/* Intrusive singly linked list: the node lives inside the caller's struct,
 * so the list never allocates and one element can be found from its node
 * with LIST_ENTRY. */
typedef struct list_node {
    struct list_node *next;
} list_node;

typedef struct list {
    list_node *head;
    list_node *tail;
    size_t count;
} list;

#define LIST_ENTRY(node, type, member) ((type *)((char *)(node) - offsetof(type, member)))

void list_init(list *l);
size_t list_count(const list *l);
bool list_is_empty(const list *l);
void list_push_front(list *l, list_node *n);
void list_push_back(list *l, list_node *n);
list_node *list_pop_front(list *l);
bool list_remove(list *l, list_node *n);
void list_reverse(list *l);

#endif
