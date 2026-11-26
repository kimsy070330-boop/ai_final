#include "raylib.h"
#include <stdlib.h>
#include <time.h>

// 1) 데이터 & 초기화 함수
void init_board(int board[][15], int solution[][15], int size) {
    int i, j;
    int heart_5[5][5] = {
        {0, 1, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}
    };
    int use_preset = (size == 5 && (rand() % 2 == 0));

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = 0;
            if (use_preset) solution[i][j] = heart_5[i][j];
            else solution[i][j] = (rand() % 10 < 6) ? 1 : 0;
        }
    }
}

// 2) 행·열 힌트 자동 생성 함수
void generate_row_hints(int solution[][15], int size, int row_hints[][8], int row_hint_counts[]) {
    int i, j;
    for (i = 0; i < size; i++) {
        int count = 0, hint_idx = 0;
        for (j = 0; j < size; j++) {
            if (solution[i][j] == 1) count++;
            else if (count > 0) {
                row_hints[i][hint_idx++] = count;
                count = 0;
            }
        }
        if (count > 0) row_hints[i][hint_idx++] = count;
        if (hint_idx == 0) {
            row_hints[i][0] = 0;
            hint_idx = 1;
        }
        row_hint_counts[i] = hint_idx;
    }
}

void generate_col_hints(int solution[][15], int size, int col_hints[][8], int col_hint_counts[]) {
    int i, j;
    for (j = 0; j < size; j++) {
        int count = 0, hint_idx = 0;
        for (i = 0; i < size; i++) {
            if (solution[i][j] == 1) count++;
            else if (count > 0) {
                col_hints[j][hint_idx++] = count;
                count = 0;
            }
        }
        if (count > 0) col_hints[j][hint_idx++] = count;
        if (hint_idx == 0) {
            col_hints[j][0] = 0;
            hint_idx = 1;
        }
        col_hint_counts[j] = hint_idx;
    }
}

// 3) 입력 및 커서 제어 함수
void handle_tile_input(char key, int *x, int *y, int board[][15], int size) {
    if (key == 'w' || key == 'W') { if (*y > 0) (*y)--; }
    else if (key == 's' || key == 'S') { if (*y < size - 1) (*y)++; }
    else if (key == 'a' || key == 'A') { if (*x > 0) (*x)--; }
    else if (key == 'd' || key == 'D') { if (*x < size - 1) (*x)++; }
    else if (key == ' ' || key == '1') { board[*y][*x] = (board[*y][*x] == 1) ? 0 : 1; }
    else if (key == 'x' || key == 'X' || key == '2') { board[*y][*x] = (board[*y][*x] == 2) ? 0 : 2; }
}

// 4) 화면 렌더링 함수
void render_game(int board[][15], int size, int x, int y,
                 int row_hints[][8], int row_hint_counts[],
                 int col_hints[][8], int col_hint_counts[]) {
    int i, j, k;
    int cell = (size == 5) ? 54 : (size == 10 ? 38 : 28);
    int start_x = 220;
    int start_y = 210;

    // 상단 열 힌트
    for (j = 0; j < size; j++) {
        int count = col_hint_counts[j];
        for (k = 0; k < count; k++) {
            int val = col_hints[j][k];
            int px = start_x + j * cell + (cell / 3);
            int py = start_y - (count - k) * 22 - 4;
            DrawText(TextFormat("%d", val), px, py, 18, (j == x) ? ORANGE : DARKBLUE);
        }
    }

    // 좌측 행 힌트
    for (i = 0; i < size; i++) {
        int count = row_hint_counts[i];
        for (k = 0; k < count; k++) {
            int val = row_hints[i][k];
            int px = start_x - (count - k) * 24 - 6;
            int py = start_y + i * cell + (cell / 4);
            DrawText(TextFormat("%d", val), px, py, 18, (i == y) ? ORANGE : DARKBLUE);
        }
    }

    // 격자판 타일
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            int px = start_x + j * cell;
            int py = start_y + i * cell;

            DrawRectangle(px + 1, py + 1, cell - 2, cell - 2, WHITE);

            if (board[i][j] == 1) {
                DrawRectangleRounded((Rectangle){(float)px + 3, (float)py + 3, (float)cell - 6, (float)cell - 6}, 0.2f, 6, DARKBLUE);
            } else if (board[i][j] == 2) {
                DrawLineEx((Vector2){(float)px + 8, (float)py + 8}, (Vector2){(float)px + cell - 8, (float)py + cell - 8}, 2.5f, RED);
                DrawLineEx((Vector2){(float)px + cell - 8, (float)py + 8}, (Vector2){(float)px + 8, (float)py + cell - 8}, 2.5f, RED);
            }

            if (i == y && j == x) {
                DrawRectangleLinesEx((Rectangle){(float)px, (float)py, (float)cell, (float)cell}, 2.5f, ORANGE);
            } else {
                DrawRectangleLines(px, py, cell, cell, LIGHTGRAY);
            }
        }
    }
}

// 5) 자동 처리 기능 함수
void auto_fill_x(int board[][15], int solution[][15], int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        int ok = 1;
        for (j = 0; j < size; j++) {
            if ((solution[i][j] == 1 && board[i][j] != 1) || (solution[i][j] == 0 && board[i][j] == 1)) ok = 0;
        }
        if (ok) {
            for (j = 0; j < size; j++) if (board[i][j] == 0) board[i][j] = 2;
        }
    }
    for (j = 0; j < size; j++) {
        int ok = 1;
        for (i = 0; i < size; i++) {
            if ((solution[i][j] == 1 && board[i][j] != 1) || (solution[i][j] == 0 && board[i][j] == 1)) ok = 0;
        }
        if (ok) {
            for (i = 0; i < size; i++) if (board[i][j] == 0) board[i][j] = 2;
        }
    }
}

// 6) 정답 판정 함수
int check_win(int board[][15], int solution[][15], int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            int filled = (board[i][j] == 1) ? 1 : 0;
            if (filled != solution[i][j]) return 0;
        }
    }
    return 1;
}

// 7) 메인 흐름 함수
int main(void) {
    int board[15][15], solution[15][15];
    int row_hints[15][8], col_hints[15][8];
    int row_hint_counts[15], col_hint_counts[15];

    int screen = 0; // 0: 난이도 선택 화면, 1: 게임 플레이 화면
    int size = 5, x = 0, y = 0;
    int lives = 3, hints = 2, is_win = 0;
    double start_time = 0.0, clear_time = 0.0;

    srand((unsigned int)time(NULL));
    InitWindow(720, 720, "NONOGRAM PUZZLE APP");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (screen == 0) {
            // 난이도 선택 버튼 클릭 처리
            Rectangle btn5 = {230, 260, 260, 60};
            Rectangle btn10 = {230, 350, 260, 60};
            Rectangle btn15 = {230, 440, 260, 60};

            int selected = 0;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, btn5)) { size = 5; selected = 1; }
                else if (CheckCollisionPointRec(mouse, btn10)) { size = 10; selected = 1; }
                else if (CheckCollisionPointRec(mouse, btn15)) { size = 15; selected = 1; }
            }

            if (selected) {
                init_board(board, solution, size);
                generate_row_hints(solution, size, row_hints, row_hint_counts);
                generate_col_hints(solution, size, col_hints, col_hint_counts);
                auto_fill_x(board, solution, size);
                x = 0; y = 0; lives = 3; hints = 2; is_win = 0;
                start_time = GetTime();
                screen = 1;
            }

            BeginDrawing();
            ClearBackground((Color){240, 244, 248, 255});
            DrawText("NONOGRAM PUZZLE", 195, 130, 34, DARKBLUE);
            DrawText("Select Difficulty", 275, 185, 20, GRAY);

            DrawRectangleRounded(btn5, 0.3f, 8, CheckCollisionPointRec(mouse, btn5) ? SKYBLUE : DARKBLUE);
            DrawText("EASY (5 x 5)", 295, 280, 20, WHITE);

            DrawRectangleRounded(btn10, 0.3f, 8, CheckCollisionPointRec(mouse, btn10) ? SKYBLUE : DARKBLUE);
            DrawText("NORMAL (10 x 10)", 275, 370, 20, WHITE);

            DrawRectangleRounded(btn15, 0.3f, 8, CheckCollisionPointRec(mouse, btn15) ? SKYBLUE : DARKBLUE);
            DrawText("HARD (15 x 15)", 285, 460, 20, WHITE);
            EndDrawing();
        } else {
            // 게임 플레이 화면
            Rectangle hintBtn = {430, 35, 120, 40};
            Rectangle menuBtn = {565, 35, 110, 40};

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, menuBtn)) {
                screen = 0;
            }

            if (!is_win && lives > 0) {
                // 힌트 버튼 클릭
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, hintBtn) && hints > 0) {
                    int found = 0;
                    for (int i = 0; i < size && !found; i++) {
                        for (int j = 0; j < size && !found; j++) {
                            if (solution[i][j] == 1 && board[i][j] != 1) {
                                board[i][j] = 1;
                                hints--;
                                found = 1;
                                auto_fill_x(board, solution, size);
                            }
                        }
                    }
                }

                // 키보드 조작
                if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) handle_tile_input('w', &x, &y, board, size);
                if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) handle_tile_input('s', &x, &y, board, size);
                if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) handle_tile_input('a', &x, &y, board, size);
                if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) handle_tile_input('d', &x, &y, board, size);
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ONE)) {
                    handle_tile_input('1', &x, &y, board, size);
                    if (board[y][x] == 1 && solution[y][x] == 0) { board[y][x] = 2; lives--; }
                    auto_fill_x(board, solution, size);
                }
                if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_TWO)) handle_tile_input('2', &x, &y, board, size);

                // 마우스 보드 클릭 조작
                int cell = (size == 5) ? 54 : (size == 10 ? 38 : 28);
                int start_x = 220, start_y = 210;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    int gx = (int)(mouse.x - start_x) / cell;
                    int gy = (int)(mouse.y - start_y) / cell;
                    if (gx >= 0 && gx < size && gy >= 0 && gy < size && mouse.x >= start_x && mouse.y >= start_y) {
                        x = gx; y = gy;
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            handle_tile_input('1', &x, &y, board, size);
                            if (board[y][x] == 1 && solution[y][x] == 0) { board[y][x] = 2; lives--; }
                            auto_fill_x(board, solution, size);
                        } else {
                            handle_tile_input('2', &x, &y, board, size);
                        }
                    }
                }

                if (check_win(board, solution, size) == 1) {
                    is_win = 1;
                    clear_time = GetTime() - start_time;
                }
            }

            BeginDrawing();
            ClearBackground((Color){245, 247, 250, 255});

            // 상단 상태바 UI
            int elapsed = is_win ? (int)clear_time : (int)(GetTime() - start_time);
            DrawText(TextFormat("SIZE: %dx%d", size, size), 35, 42, 22, DARKBLUE);
            DrawText(TextFormat("LIFE: %d", lives), 185, 42, 22, RED);
            DrawText(TextFormat("TIME: %02d:%02d", elapsed / 60, elapsed % 60), 295, 42, 22, DARKGRAY);

            DrawRectangleRounded(hintBtn, 0.3f, 8, (hints > 0) ? ORANGE : LIGHTGRAY);
            DrawText(TextFormat("HINT (%d)", hints), 448, 46, 18, WHITE);

            DrawRectangleRounded(menuBtn, 0.3f, 8, GRAY);
            DrawText("MENU", 592, 46, 18, WHITE);

            render_game(board, size, x, y, row_hints, row_hint_counts, col_hints, col_hint_counts);

            DrawText("Left Click: Fill  |  Right Click: X Mark  |  Arrow/WASD: Move", 125, 675, 17, GRAY);

            // 클리어 / 게임 오버 오버레이 배너
            if (is_win) {
                DrawRectangle(0, 300, 720, 120, (Color){25, 135, 84, 235});
                DrawText(TextFormat("PUZZLE CLEAR! (%02d:%02d)", elapsed / 60, elapsed % 60), 195, 342, 32, WHITE);
            } else if (lives <= 0) {
                DrawRectangle(0, 300, 720, 120, (Color){220, 53, 69, 235});
                DrawText("GAME OVER! Click MENU to Retry", 155, 342, 28, WHITE);
            }

            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
