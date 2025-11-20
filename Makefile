CC      := cc
CFLAGS  := -Wall -Wextra -std=c11 -Iinclude -I.

PROGRAM := csharpier

# All .c files in src
SRC_SRCS := $(wildcard src/*.c)
SRC_OBJS := $(SRC_SRCS:.c=.o)

# Tests
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(patsubst tests/%.c,tests/%,$(TEST_SRCS))

.PHONY: all build test clean

# Default target: build main program
all: build

build: $(PROGRAM)

$(PROGRAM): main.o $(SRC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---------- Tests ----------

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
