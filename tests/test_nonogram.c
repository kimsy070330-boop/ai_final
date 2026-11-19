#include "nonogram.h"

#include <stdio.h>
#include <string.h>

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK_TRUE(condition)                                                   \
    do {                                                                         \
        ++tests_run;                                                             \
        if (!(condition)) {                                                      \
            ++tests_failed;                                                      \
            printf("FAIL: %s (line %d)\n", #condition, __LINE__);              \
        }                                                                        \
    } while (0)

#define CHECK_FALSE(condition) CHECK_TRUE(!(condition))

static void test_init(void) {
    unsigned char answer[4][4] = {
        {1, 1, 0, 0},
        {1, 0, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };
    Nonogram game;

    CHECK_TRUE(nonogram_init(&game, 4, answer));
    CHECK_TRUE(game.size == 4);
    CHECK_TRUE(game.answer[0][0] == 1);
    CHECK_TRUE(game.answer[0][2] == 0);
    CHECK_FALSE(nonogram_init(NULL, 4, answer));
    CHECK_FALSE(nonogram_init(&game, 0, answer));
    CHECK_FALSE(nonogram_init(&game, MAX_SIZE + 1, answer));
}

static void test_valid_position(void) {
    unsigned char answer[MAX_SIZE][MAX_SIZE] = {{0}};
    Nonogram game;

    nonogram_init(&game, 3, answer);
    CHECK_TRUE(nonogram_is_valid_position(&game, 0, 0));
    CHECK_TRUE(nonogram_is_valid_position(&game, 2, 2));
    CHECK_FALSE(nonogram_is_valid_position(&game, -1, 0));
    CHECK_FALSE(nonogram_is_valid_position(&game, 0, 3));
    CHECK_FALSE(nonogram_is_valid_position(NULL, 0, 0));
}

static void test_set_and_toggle_cell(void) {
    unsigned char answer[MAX_SIZE][MAX_SIZE] = {{0}};
    Nonogram game;

    nonogram_init(&game, 3, answer);
    CHECK_TRUE(nonogram_set_cell(&game, 1, 2, CELL_FILLED));
    CHECK_TRUE(game.board[1][2] == CELL_FILLED);
    CHECK_FALSE(nonogram_set_cell(&game, 3, 0, CELL_FILLED));

    CHECK_TRUE(nonogram_toggle_cell(&game, 1, 2));
    CHECK_TRUE(game.board[1][2] == CELL_MARKED);
    CHECK_TRUE(nonogram_toggle_cell(&game, 1, 2));
    CHECK_TRUE(game.board[1][2] == CELL_EMPTY);
    CHECK_TRUE(nonogram_toggle_cell(&game, 1, 2));
    CHECK_TRUE(game.board[1][2] == CELL_FILLED);
    CHECK_FALSE(nonogram_toggle_cell(&game, -1, 2));
}

static void test_row_hint(void) {
    unsigned char answer[5][5] = {
        {1, 1, 0, 1, 1},
        {0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 1, 1, 1},
        {0, 1, 0, 1, 0}
    };
    Nonogram game;
    int hint[MAX_SIZE];
    int count = -1;

    nonogram_init(&game, 5, answer);
    CHECK_TRUE(nonogram_get_row_hint(&game, 0, hint, &count));
    CHECK_TRUE(count == 2);
    CHECK_TRUE(hint[0] == 2 && hint[1] == 2);

    CHECK_TRUE(nonogram_get_row_hint(&game, 1, hint, &count));
    CHECK_TRUE(count == 0);

    CHECK_TRUE(nonogram_get_row_hint(&game, 2, hint, &count));
    CHECK_TRUE(count == 2);
    CHECK_TRUE(hint[0] == 1 && hint[1] == 2);

    CHECK_FALSE(nonogram_get_row_hint(&game, 5, hint, &count));
}

static void test_col_hint(void) {
    unsigned char answer[4][4] = {
        {1, 0, 1, 1},
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 1}
    };
    Nonogram game;
    int hint[MAX_SIZE];
    int count = -1;

    nonogram_init(&game, 4, answer);
    CHECK_TRUE(nonogram_get_col_hint(&game, 0, hint, &count));
    CHECK_TRUE(count == 1 && hint[0] == 2);

    CHECK_TRUE(nonogram_get_col_hint(&game, 1, hint, &count));
    CHECK_TRUE(count == 1 && hint[0] == 2);

    CHECK_TRUE(nonogram_get_col_hint(&game, 2, hint, &count));
    CHECK_TRUE(count == 1 && hint[0] == 1);

    CHECK_FALSE(nonogram_get_col_hint(&game, 4, hint, &count));
}

static void test_check_row_and_col(void) {
    unsigned char answer[3][3] = {
        {1, 1, 0},
        {0, 1, 0},
        {1, 0, 1}
    };
    Nonogram game;

    nonogram_init(&game, 3, answer);
    CHECK_FALSE(nonogram_check_row(&game, 0));

    nonogram_set_cell(&game, 0, 0, CELL_FILLED);
    nonogram_set_cell(&game, 0, 1, CELL_FILLED);
    CHECK_TRUE(nonogram_check_row(&game, 0));
    CHECK_TRUE(nonogram_check_col(&game, 0));

    nonogram_set_cell(&game, 1, 1, CELL_FILLED);
    CHECK_TRUE(nonogram_check_col(&game, 1));
    CHECK_FALSE(nonogram_check_row(&game, 99));
}

static void test_is_solved(void) {
    unsigned char answer[3][3] = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 1, 0}
    };
    Nonogram game;

    nonogram_init(&game, 3, answer);
    CHECK_FALSE(nonogram_is_solved(&game));

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (answer[row][col]) {
                nonogram_set_cell(&game, row, col, CELL_FILLED);
            }
        }
    }

    CHECK_TRUE(nonogram_is_solved(&game));

    nonogram_set_cell(&game, 0, 0, CELL_MARKED);
    CHECK_FALSE(nonogram_is_solved(&game));
}

static void test_reset(void) {
    unsigned char answer[MAX_SIZE][MAX_SIZE] = {{1}};
    Nonogram game;

    nonogram_init(&game, 1, answer);
    nonogram_set_cell(&game, 0, 0, CELL_FILLED);
    nonogram_reset(&game);

    CHECK_TRUE(game.board[0][0] == CELL_EMPTY);
    CHECK_TRUE(game.answer[0][0] == 1);
}

int main(void) {
    test_init();
    test_valid_position();
    test_set_and_toggle_cell();
    test_row_hint();
    test_col_hint();
    test_check_row_and_col();
    test_is_solved();
    test_reset();

    printf("테스트 실행: %d개, 실패: %d개\n", tests_run, tests_failed);
    return tests_failed == 0 ? 0 : 1;
}
