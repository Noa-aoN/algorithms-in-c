#include "ring_buffer.h"

#include <string.h>

#include "minitest.h"

static void test_init_requires_power_of_two(void) {
    ring_buffer rb;
    uint8_t s[8];
    MT_CHECK(!ring_buffer_init(&rb, NULL, 8));
    MT_CHECK(!ring_buffer_init(&rb, s, 0));
    MT_CHECK(!ring_buffer_init(&rb, s, 6));
    MT_CHECK(ring_buffer_init(&rb, s, 8));
    MT_CHECK(ring_buffer_capacity(&rb) == 8);
    MT_CHECK(ring_buffer_count(&rb) == 0);
    MT_CHECK(ring_buffer_is_empty(&rb));
    MT_CHECK(!ring_buffer_is_full(&rb));
}

static void test_push_pop_fifo(void) {
    ring_buffer rb;
    uint8_t s[4], v;
    ring_buffer_init(&rb, s, 4);
    MT_CHECK(ring_buffer_push(&rb, 10));
    MT_CHECK(ring_buffer_push(&rb, 20));
    MT_CHECK(ring_buffer_push(&rb, 30));
    MT_CHECK(ring_buffer_push(&rb, 40));
    MT_CHECK(ring_buffer_is_full(&rb));
    MT_CHECK(!ring_buffer_push(&rb, 50));
    MT_CHECK(ring_buffer_count(&rb) == 4);
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 10);
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 20);
    MT_CHECK(ring_buffer_push(&rb, 50));
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 30);
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 40);
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 50);
    MT_CHECK(!ring_buffer_pop(&rb, &v));
    MT_CHECK(ring_buffer_is_empty(&rb));
}

static void test_bulk_write_read_wraps(void) {
    ring_buffer rb;
    uint8_t s[8], out[8];
    const uint8_t in[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ring_buffer_init(&rb, s, 8);
    MT_CHECK(ring_buffer_write(&rb, in, 5) == 5);
    MT_CHECK(ring_buffer_read(&rb, out, 3) == 3);
    MT_CHECK(memcmp(out, in, 3) == 0);
    MT_CHECK(ring_buffer_write(&rb, in + 5, 5) == 5);
    MT_CHECK(ring_buffer_write(&rb, in, 1) == 1);
    MT_CHECK(ring_buffer_is_full(&rb));
    MT_CHECK(ring_buffer_write(&rb, in, 1) == 0);
    MT_CHECK(ring_buffer_read(&rb, out, 8) == 8);
    MT_CHECK(memcmp(out, in + 3, 7) == 0 && out[7] == 1);
    MT_CHECK(ring_buffer_read(&rb, out, 8) == 0);
}

static void test_index_wraparound_is_safe(void) {
    ring_buffer rb;
    uint8_t s[2], v;
    ring_buffer_init(&rb, s, 2);
    rb.head = SIZE_MAX - 1;
    rb.tail = SIZE_MAX - 1;
    MT_CHECK(ring_buffer_push(&rb, 1));
    MT_CHECK(ring_buffer_push(&rb, 2));
    MT_CHECK(ring_buffer_is_full(&rb));
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 1);
    MT_CHECK(ring_buffer_push(&rb, 3));
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 2);
    MT_CHECK(ring_buffer_pop(&rb, &v) && v == 3);
    MT_CHECK(ring_buffer_is_empty(&rb));
    MT_CHECK(rb.head == 1 && rb.tail == 1);
}

int main(void) {
    MT_RUN(test_init_requires_power_of_two);
    MT_RUN(test_push_pop_fifo);
    MT_RUN(test_bulk_write_read_wraps);
    MT_RUN(test_index_wraparound_is_safe);
    return mt_report("ring_buffer");
}
