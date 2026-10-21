CC      ?= cc
BUILD   ?= build
CFLAGS  ?= -O2
WARN     = -std=c11 -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion -Wstrict-prototypes
SAN      = -fsanitize=address,undefined -fno-omit-frame-pointer
CROSS   ?= arm-none-eabi-
MCU     ?= -mcpu=cortex-m4 -mthumb

TEST_SRC = $(wildcard */*_test.c)
TEST_BIN = $(TEST_SRC:%.c=$(BUILD)/%)
LIB_SRC  = $(filter-out %_test.c,$(wildcard */*.c))
CROSS_OBJ = $(LIB_SRC:%.c=$(BUILD)/cross/%.o)

.PHONY: all test asan valgrind cross clean

all: $(TEST_BIN)

$(BUILD)/%_test: %_test.c %.c %.h tests/minitest.h
	@mkdir -p $(@D)
	$(CC) $(WARN) $(CFLAGS) -I$(<D) -Itests -o $@ $< $(word 2,$^) $(LDFLAGS)

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do ./$$t || exit 1; done

asan:
	$(MAKE) test BUILD=$(BUILD)/asan CFLAGS="-O1 -g $(SAN)" LDFLAGS="$(SAN)"

valgrind: $(TEST_BIN)
	@for t in $(TEST_BIN); do \
		valgrind -q --error-exitcode=1 --leak-check=full ./$$t || exit 1; \
	done

$(BUILD)/cross/%.o: %.c %.h
	@mkdir -p $(@D)
	$(CROSS)gcc $(WARN) $(MCU) -Os -I$(<D) -c -o $@ $<

cross: $(CROSS_OBJ)
	$(CROSS)size -t $^

clean:
	rm -rf $(BUILD)

.DELETE_ON_ERROR:
