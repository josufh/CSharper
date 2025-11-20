CC      := cc
CFLAGS  := -Wall -Wextra -std=c11 -Iinclude -I.

PROGRAM := csharpier

SRC_SRCS := $(wildcard src/*.c)
SRC_OBJS := $(SRC_SRCS:.c=.o)

TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(patsubst tests/%.c,tests/%,$(TEST_SRCS))

.PHONY: all build test clean

all: build

build: $(PROGRAM)

$(PROGRAM): main.o $(SRC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BINS)
	@set -e; \
	for t in $(TEST_BINS); do \
	    echo "Running $$t"; \
	    $$t; \
	done
	@rm -f $(TEST_BINS)

tests/%: tests/%.c $(SRC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_OBJS) $(TEST_BINS) $(PROGRAM) main.o
