#ifndef NONOGRAM_H
#define NONOGRAM_H

#define MAX_SIZE 20

typedef enum {
    CELL_EMPTY = 0,
    CELL_FILLED = 1,
    CELL_MARKED = 2
} CellState;

typedef struct {
    int size;
    unsigned char answer[MAX_SIZE][MAX_SIZE];
    CellState board[MAX_SIZE][MAX_SIZE];
} Nonogram;

int nonogram_init(Nonogram *game, int size,
                  const unsigned char answer[MAX_SIZE][MAX_SIZE]);
int nonogram_is_valid_position(const Nonogram *game, int row, int col);
int nonogram_set_cell(Nonogram *game, int row, int col, CellState state);
int nonogram_toggle_cell(Nonogram *game, int row, int col);
int nonogram_get_row_hint(const Nonogram *game, int row,
                          int hint[MAX_SIZE], int *hint_count);
int nonogram_get_col_hint(const Nonogram *game, int col,
                          int hint[MAX_SIZE], int *hint_count);
int nonogram_check_row(const Nonogram *game, int row);
int nonogram_check_col(const Nonogram *game, int col);
int nonogram_is_solved(const Nonogram *game);
void nonogram_reset(Nonogram *game);

#endif
