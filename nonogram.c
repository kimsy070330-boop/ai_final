#include "nonogram.h"

#include <string.h>

int nonogram_init(Nonogram *game, int size,
                  const unsigned char answer[MAX_SIZE][MAX_SIZE]) {
    if (game == NULL || answer == NULL || size < 1 || size > MAX_SIZE) {
        return 0;
    }

    memset(game, 0, sizeof(*game));
    game->size = size;

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            game->answer[row][col] = answer[row][col] ? 1 : 0;
            game->board[row][col] = CELL_EMPTY;
        }
    }
    return 1;
}

int nonogram_is_valid_position(const Nonogram *game, int row, int col) {
    return game != NULL && row >= 0 && col >= 0 &&
           row < game->size && col < game->size;
}

int nonogram_set_cell(Nonogram *game, int row, int col, CellState state) {
    if (!nonogram_is_valid_position(game, row, col)) {
        return 0;
    }
    if (state != CELL_EMPTY && state != CELL_FILLED && state != CELL_MARKED) {
        return 0;
    }

    game->board[row][col] = state;
    return 1;
}

int nonogram_toggle_cell(Nonogram *game, int row, int col) {
    if (!nonogram_is_valid_position(game, row, col)) {
        return 0;
    }

    if (game->board[row][col] == CELL_EMPTY) {
        game->board[row][col] = CELL_FILLED;
    } else if (game->board[row][col] == CELL_FILLED) {
        game->board[row][col] = CELL_MARKED;
    } else {
        game->board[row][col] = CELL_EMPTY;
    }
    return 1;
}

int nonogram_get_row_hint(const Nonogram *game, int row,
                          int hint[MAX_SIZE], int *hint_count) {
    if (game == NULL || hint == NULL || hint_count == NULL ||
        row < 0 || row >= game->size) {
        return 0;
    }

    *hint_count = 0;
    int run = 0;

    for (int col = 0; col < game->size; ++col) {
        if (game->answer[row][col]) {
            ++run;
        } else if (run > 0) {
            hint[(*hint_count)++] = run;
            run = 0;
        }
    }

    if (run > 0) {
        hint[(*hint_count)++] = run;
    }
    return 1;
}

int nonogram_get_col_hint(const Nonogram *game, int col,
                          int hint[MAX_SIZE], int *hint_count) {
    if (game == NULL || hint == NULL || hint_count == NULL ||
        col < 0 || col >= game->size) {
        return 0;
    }

    *hint_count = 0;
    int run = 0;

    for (int row = 0; row < game->size; ++row) {
        if (game->answer[row][col]) {
            ++run;
        } else if (run > 0) {
            hint[(*hint_count)++] = run;
            run = 0;
        }
    }

    if (run > 0) {
        hint[(*hint_count)++] = run;
    }
    return 1;
}

int nonogram_check_row(const Nonogram *game, int row) {
    if (game == NULL || row < 0 || row >= game->size) {
        return 0;
    }

    for (int col = 0; col < game->size; ++col) {
        int filled = game->board[row][col] == CELL_FILLED;
        if (filled != game->answer[row][col]) {
            return 0;
        }
    }
    return 1;
}

int nonogram_check_col(const Nonogram *game, int col) {
    if (game == NULL || col < 0 || col >= game->size) {
        return 0;
    }

    for (int row = 0; row < game->size; ++row) {
        int filled = game->board[row][col] == CELL_FILLED;
        if (filled != game->answer[row][col]) {
            return 0;
        }
    }
    return 1;
}

int nonogram_is_solved(const Nonogram *game) {
    if (game == NULL || game->size < 1) {
        return 0;
    }

    for (int row = 0; row < game->size; ++row) {
        if (!nonogram_check_row(game, row)) {
            return 0;
        }
    }
    return 1;
}

void nonogram_reset(Nonogram *game) {
    if (game == NULL) {
        return;
    }

    for (int row = 0; row < game->size; ++row) {
        for (int col = 0; col < game->size; ++col) {
            game->board[row][col] = CELL_EMPTY;
        }
    }
}
