CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror

.PHONY: all test clean

all: test

test: tests/test_nonogram
	./tests/test_nonogram

tests/test_nonogram: tests/test_nonogram.c nonogram.c nonogram.h
	mkdir -p tests
	$(CC) $(CFLAGS) tests/test_nonogram.c nonogram.c -o tests/test_nonogram

clean:
	rm -f tests/test_nonogram
