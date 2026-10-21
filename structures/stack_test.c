#include "stack.h"

#include "minitest.h"

static void test_init_rejects_bad_arguments(void) {
    int buf[4];
    stack s;
    MT_CHECK(!stack_init(&s, NULL, sizeof(int), 4));
    MT_CHECK(!stack_init(&s, buf, 0, 4));
    MT_CHECK(!stack_init(&s, buf, sizeof(int), 0));
    MT_CHECK(stack_init(&s, buf, sizeof(int), 4));
    MT_CHECK(stack_is_empty(&s) && !stack_is_full(&s) && stack_count(&s) == 0);
}

static void test_lifo_order_and_bounds(void) {
    int buf[3];
    int v = 0;
    stack s;
    stack_init(&s, buf, sizeof(int), 3);
    MT_CHECK(!stack_pop(&s, &v));
    MT_CHECK(!stack_peek(&s, &v));
    for (int i = 1; i <= 3; i++) {
        MT_CHECK(stack_push(&s, &i));
    }
    v = 4;
    MT_CHECK(stack_is_full(&s));
    MT_CHECK(!stack_push(&s, &v));
    MT_CHECK(stack_peek(&s, &v) && v == 3);
    MT_CHECK(stack_count(&s) == 3);
    MT_CHECK(stack_pop(&s, &v) && v == 3);
    MT_CHECK(stack_pop(&s, &v) && v == 2);
    MT_CHECK(stack_pop(&s, &v) && v == 1);
    MT_CHECK(!stack_pop(&s, &v));
    MT_CHECK(stack_is_empty(&s));
}

typedef struct point {
    int16_t x;
    int16_t y;
    uint8_t tag;
} point;

static void test_struct_elements_keep_their_bytes(void) {
    point buf[2];
    point p = {-5, 7, 0xAB};
    point q = {0, 0, 0};
    stack s;
    stack_init(&s, buf, sizeof(point), 2);
    MT_CHECK(stack_push(&s, &p));
    p.x = 100;
    MT_CHECK(stack_pop(&s, &q));
    MT_CHECK(q.x == -5 && q.y == 7 && q.tag == 0xAB);
}

static void test_reuse_after_drain(void) {
    uint8_t buf[2];
    uint8_t v;
    stack s;
    stack_init(&s, buf, 1, 2);
    for (int round = 0; round < 1000; round++) {
        v = (uint8_t)round;
        MT_CHECK(stack_push(&s, &v));
        v = (uint8_t)(round + 1);
        MT_CHECK(stack_push(&s, &v));
        MT_CHECK(stack_pop(&s, &v) && v == (uint8_t)(round + 1));
        MT_CHECK(stack_pop(&s, &v) && v == (uint8_t)round);
    }
}

int main(void) {
    MT_RUN(test_init_rejects_bad_arguments);
    MT_RUN(test_lifo_order_and_bounds);
    MT_RUN(test_struct_elements_keep_their_bytes);
    MT_RUN(test_reuse_after_drain);
    return mt_report("stack");
}
