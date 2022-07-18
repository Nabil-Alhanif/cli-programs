#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <vector>
using namespace std;

enum direction {
    NONE = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
};

class Tetromino {
    public:
        vector<vector<char>> shape;

        Tetromino()
        {
            shape.resize(4, vector<char>(4, ' '));
        }

        void rotate()
        {
            vector<vector<char>> cpy(4, vector<char>(4));
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++)
                    cpy[row][col] = this->shape[3 - col][row];
            }

            int col_norm = -1, row_norm = -1;
            for (int col = 0; col < 4; col++) {
                for (int row = 0; row < 4; row++) {
                    if (cpy[row][col] != ' ') {
                        col_norm = col;
                        break;
                    }
                }
                if (col_norm != -1)
                    break;
            }
            for (int row = 3; row >= 0; row--) {
                for (int col = 0; col < 4; col++) {
                    if (cpy[row][col] != ' ') {
                        row_norm = 3 - row;
                        break;
                    }
                }
                if (row_norm != -1)
                    break;
            }

            // You know, just in case :shrug
            if ((col_norm == -1) || (row_norm == -1)) {
                this->shape = cpy;
                return;
            }

            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    this->shape[row][col] = cpy[(row + 4 - row_norm) % 4][(col + col_norm) % 4];
                }
            }
        }
};

class Tetris {
    public:
        vector<Tetromino> tetrominos;
        vector<vector<char>> board;

        Tetris()
        {
            this->tetrominos.resize(7);
            this->tetrominos[0].shape = {{'$',' ',' ',' '},
                                         {'$',' ',' ',' '},
                                         {'$',' ',' ',' '},
                                         {'$',' ',' ',' '}};

            this->tetrominos[1].shape = {{' ',' ',' ',' '},
                                         {' ',' ',' ',' '},
                                         {'$','$',' ',' '},
                                         {'$','$',' ',' '}};

            this->tetrominos[2].shape = {{' ',' ',' ',' '},
                                         {'$',' ',' ',' '},
                                         {'$',' ',' ',' '},
                                         {'$','$',' ',' '}};

            this->tetrominos[3].shape = {{' ',' ',' ',' '},
                                         {' ','$',' ',' '},
                                         {' ','$',' ',' '},
                                         {'$','$',' ',' '}};

            this->tetrominos[4].shape = {{' ',' ',' ',' '},
                                         {' ',' ',' ',' '},
                                         {' ','$',' ',' '},
                                         {'$','$','$',' '}};

            this->tetrominos[5].shape = {{' ',' ',' ',' '},
                                         {' ',' ',' ',' '},
                                         {'$','$',' ',' '},
                                         {' ','$','$',' '}};

            this->tetrominos[6].shape = {{' ',' ',' ',' '},
                                         {' ',' ',' ',' '},
                                         {' ','$','$',' '},
                                         {'$','$',' ',' '}};

            this->board.resize(16, vector<char> (32, ' '));
        }

        // Just for test
        void test()
        {
            for (auto &i:tetrominos) {
                for (auto &row:i.shape) {
                    for (auto &col:row) {
                        if (col == ' ')
                            col = '-';
                        else col = '#';
                    }
                }
            }
        }

        void run() {};
};

int main()
{
    /* Initialize ncurses */
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);

    auto t1 = chrono::steady_clock::now();
    auto t2 = chrono::steady_clock::now();

    int ch;
    int startx = 0, starty = 0;
    direction dir = NONE;

    Tetris tetris;

    bool is_game_running = 1;
    bool should_calculate_movement = 0;

    while (is_game_running) {
        ch = getch();
        if (ch == KEY_DOWN)
            dir = DOWN;
        else if (ch == KEY_LEFT)
            dir = LEFT;
        else if (ch == KEY_RIGHT)
            dir = RIGHT;
        else if (ch == KEY_UP)
            dir = UP;

        t2 = chrono::steady_clock::now();
        chrono::duration<float> elapsed_time = t2 - t1;

        // We should calculate user keypress every 0.1 seconds
        if (elapsed_time.count() > 0.1f) {
            if (dir != NONE) {
                should_calculate_movement = 1;
                t1 = t2;
            }
        }
        // And automatic downward movement every 1.0 seconds
        if (elapsed_time.count() > 1.0f) {
            if (dir == NONE) {
                should_calculate_movement = 1;
                dir = DOWN;
                t1 = t2;
            }
        }

        if (should_calculate_movement) {
            erase();

            if (dir == UP)
                starty--;
            else if (dir == DOWN)
                starty++;
            else if (dir == LEFT)
                startx--;
            else if (dir == RIGHT)
                startx++;

            dir = NONE;
        }
    }

    endwin();
    return 0;
}
