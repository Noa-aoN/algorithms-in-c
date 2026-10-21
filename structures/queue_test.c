#include "queue.h"

#include "minitest.h"

static void test_init_rejects_bad_arguments(void) {
    int buf[4];
    queue q;
    MT_CHECK(!queue_init(&q, NULL, sizeof(int), 4));
    MT_CHECK(!queue_init(&q, buf, 0, 4));
    MT_CHECK(!queue_init(&q, buf, sizeof(int), 0));
    MT_CHECK(queue_init(&q, buf, sizeof(int), 4));
    MT_CHECK(queue_is_empty(&q) && !queue_is_full(&q));
}

static void test_fifo_order_and_bounds(void) {
    int buf[3];
    int v = 0;
    queue q;
    queue_init(&q, buf, sizeof(int), 3);
    MT_CHECK(!queue_pop(&q, &v));
    MT_CHECK(!queue_peek(&q, &v));
    for (int i = 1; i <= 3; i++) {
        MT_CHECK(queue_push(&q, &i));
    }
    v = 4;
    MT_CHECK(queue_is_full(&q));
    MT_CHECK(!queue_push(&q, &v));
    MT_CHECK(queue_peek(&q, &v) && v == 1);
    MT_CHECK(queue_pop(&q, &v) && v == 1);
    MT_CHECK(queue_pop(&q, &v) && v == 2);
    MT_CHECK(queue_pop(&q, &v) && v == 3);
    MT_CHECK(!queue_pop(&q, &v));
}

static void test_wraps_around_with_odd_capacity(void) {
    uint16_t buf[5];
    uint16_t v;
    queue q;
    queue_init(&q, buf, sizeof(uint16_t), 5);
    uint16_t next_in = 0, next_out = 0;
    for (int round = 0; round < 500; round++) {
        while (!queue_is_full(&q)) {
            MT_CHECK(queue_push(&q, &next_in));
            next_in++;
        }
        for (int k = 0; k < 3; k++) {
            MT_CHECK(queue_pop(&q, &v) && v == next_out);
            next_out++;
        }
    }
    MT_CHECK(queue_count(&q) == 2);
    MT_CHECK(queue_pop(&q, &v) && v == next_out);
    MT_CHECK(queue_pop(&q, &v) && v == next_out + 1);
    MT_CHECK(queue_is_empty(&q));
}

typedef struct msg {
    uint32_t id;
    uint8_t payload[3];
} msg;

static void test_struct_elements(void) {
    msg buf[2];
    msg a = {7, {1, 2, 3}};
    msg b = {0, {0, 0, 0}};
    queue q;
    queue_init(&q, buf, sizeof(msg), 2);
    MT_CHECK(queue_push(&q, &a));
    a.id = 99;
    MT_CHECK(queue_pop(&q, &b));
    MT_CHECK(b.id == 7 && b.payload[0] == 1 && b.payload[2] == 3);
}

int main(void) {
    MT_RUN(test_init_rejects_bad_arguments);
    MT_RUN(test_fifo_order_and_bounds);
    MT_RUN(test_wraps_around_with_odd_capacity);
    MT_RUN(test_struct_elements);
    return mt_report("queue");
}
