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

    answer[0][0] = 1;
    answer[0][1] = 1;
    answer[1][0] = 1;
    answer[1][2] = 1;

    CHECK(nonogram_init(&game, 4, answer) == 1);
    CHECK(game.size == 4);
    CHECK(game.answer[0][0] == 1);
    CHECK(game.answer[0][2] == 0);
    CHECK(game.answer[1][2] == 1);
    CHECK(game.board[0][0] == CELL_EMPTY);

    CHECK(nonogram_init(NULL, 4, answer) == 0);
    CHECK(nonogram_init(&game, 0, answer) == 0);
    CHECK(nonogram_init(&game, MAX_SIZE + 1, answer) == 0);
    CHECK(nonogram_init(&game, 4, NULL) == 0);

    printf("nonogram_init: %d tests, %d failures\n", tests_run, tests_failed);
    return tests_failed == 0 ? 0 : 1;
}
