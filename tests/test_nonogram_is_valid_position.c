#include "nonogram.h"

#include <stdio.h>

static int tests_run;
static int tests_failed;

#define CHECK(condition)                                                        \
    do {                                                                         \
        ++tests_run;                                                             \
        if (!(condition)) {                                                      \
            ++tests_failed;                                                      \
            printf("FAIL: %s (line %d)\n", #condition, __LINE__);              \
        }                                                                        \
    } while (0)

int main(void) {
    unsigned char answer[MAX_SIZE][MAX_SIZE] = {{0}};
    Nonogram game;

    CHECK(nonogram_init(&game, 3, answer) == 1);
    CHECK(nonogram_is_valid_position(&game, 0, 0) == 1);
    CHECK(nonogram_is_valid_position(&game, 2, 2) == 1);
    CHECK(nonogram_is_valid_position(&game, -1, 0) == 0);
    CHECK(nonogram_is_valid_position(&game, 0, -1) == 0);
    CHECK(nonogram_is_valid_position(&game, 3, 0) == 0);
    CHECK(nonogram_is_valid_position(&game, 0, 3) == 0);
    CHECK(nonogram_is_valid_position(NULL, 0, 0) == 0);

    printf("nonogram_is_valid_position: %d tests, %d failures\n",
           tests_run, tests_failed);
    return tests_failed == 0 ? 0 : 1;
}
