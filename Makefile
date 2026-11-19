CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror

.PHONY: all test test-init test-valid-position clean

all: test

test: test-init test-valid-position

test-init: tests/test_nonogram_init
	./tests/test_nonogram_init

test-valid-position: tests/test_nonogram_is_valid_position
	./tests/test_nonogram_is_valid_position

tests/test_nonogram_init: tests/test_nonogram_init.c nonogram.c nonogram.h
	$(CC) $(CFLAGS) tests/test_nonogram_init.c nonogram.c -o $@

tests/test_nonogram_is_valid_position: tests/test_nonogram_is_valid_position.c nonogram.c nonogram.h
	$(CC) $(CFLAGS) tests/test_nonogram_is_valid_position.c nonogram.c -o $@

clean:
	rm -f tests/test_nonogram_init tests/test_nonogram_is_valid_position
