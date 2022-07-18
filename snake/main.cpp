#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <vector>
using namespace std;

enum direction {
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8
};

enum chars {
    UP_C = '^',
    DOWN_C = 'V',
    LEFT_C = '<',
    RIGHT_C = '>',
    FOOD = '$',
    EMPTY = ' ',
    BORDER = '#'
};

struct Segment {
    int row;
    int col;
    direction dir;
};

class Game {
    public:
        int score;
        int food_value;

        int max_row;
        int max_col;

        vector<vector<char>> board;
        vector<Segment> snake;

        Game(int width, int height, int food_val = 10)
        {
            this->score = 0;
            this->food_value = food_val;

            this->max_row = height - 2;
            this->max_col = width - 2;

            this->board.resize(this->max_row);
            for (auto &i:this->board)
                i.resize(this->max_col, EMPTY);

            int start_row = height / 2;
            int start_col = (width / 2) - 1;

            this->snake.resize(3);
            for (int i = 0; i < 3; i++) {
                this->snake[i].row = start_row;
                this->snake[i].col = start_col + i;
                this->snake[i].dir = LEFT;

                // Draw them on the board
                this->board[start_row][start_col + i] = LEFT_C;
            }

            this->generateFood();
        }

        bool move()
        {
            bool is_food_eaten = 0;
            bool has_hit_self = 0;

            int tail_row, tail_col;
            tail_row = this->snake.back().row;
            tail_col = this->snake.back().col;

            direction last_move = this->snake[0].dir;
            for (auto &i:this->snake) {
                this->board[i.row][i.col] = EMPTY;

                char ch;
                switch (i.dir) {
                    case UP:
                        i.row--;
                        ch = UP_C;
                        break;
                    case DOWN:
                        i.row++;
                        ch = DOWN_C;
                        break;
                    case LEFT:
                        i.col--;
                        ch = LEFT_C;
                        break;
                    case RIGHT:
                        i.col++;
                        ch = RIGHT_C;
                        break;
                }

                // refit current segment position
                i.row = (i.row + this->max_row) % this->max_row;
                i.col = (i.col + this->max_col) % this->max_col;

                // Check if we will eat the food after current movement
                if (this->board[i.row][i.col] == FOOD)
                    is_food_eaten = 1;
                // If not, check if we will hit ourselves
                else if (this->board[i.row][i.col] != EMPTY) {
                    has_hit_self = 1;
                    break;
                }

                this->board[i.row][i.col] = ch;

                swap(last_move, i.dir);
            }

            if (is_food_eaten) {
                this->score += this->food_value;
                this->generateFood();

                // Add new segment
                this->snake.push_back(Segment { .row = tail_row, .col = tail_col, .dir = last_move });
            }

            return (!has_hit_self);
        }

        void generateFood()
        {
            int row, col;

            do {
                row = rand() % this->max_row;
                col = rand() % this->max_col;
            } while (this->board[row][col] != EMPTY);

            this->board[row][col] = FOOD;
        }

        bool run()
        {
            return 1;
        }
};

int main()
{
    /* Initialize ncurses */
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);

    srand((unsigned)time(0));

    auto t1 = chrono::steady_clock::now();
    auto t2 = chrono::steady_clock::now();

    int ch;
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);

    Game game(max_col - 2, max_row - 3);

    bool is_game_running = 1;

    while (is_game_running) {
        ch = getch();
        if ((ch == KEY_UP) && ((game.snake[0].dir == LEFT) || (game.snake[0].dir == RIGHT)))
            game.snake[0].dir = UP;
        else if ((ch == KEY_DOWN) && ((game.snake[0].dir == LEFT) || (game.snake[0].dir == RIGHT)))
            game.snake[0].dir = DOWN;
        else if ((ch == KEY_LEFT) && ((game.snake[0].dir == UP) || (game.snake[0].dir == DOWN)))
            game.snake[0].dir = LEFT;
        else if ((ch == KEY_RIGHT) && ((game.snake[0].dir == UP) || (game.snake[0].dir == DOWN)))
            game.snake[0].dir = RIGHT;


        t2 = chrono::steady_clock::now();
        chrono::duration<float> elapsed_time = t2 - t1;

        // Do calculation every .1 seconds
        if (elapsed_time.count() > 0.1f) {
            is_game_running = game.move();

            erase();
            mvprintw(0, 0, "Current score: %d", game.score);

            // Draw border
            for (int i = 0; i < max_col; i++) {
                mvaddch(1, i, BORDER); // Upper
                mvaddch(max_row - 1, i, BORDER); // Lower
            }

            for (int i = 1; i < max_row; i++) {
                mvaddch(i, 0, BORDER); // Left
                mvaddch(i, max_col - 1, BORDER); // Right
            }

            for (int row = 0; row < game.max_row; row++) {
                for (int col = 1; col < game.max_col; col++)
                    mvaddch(row + 2, col + 1, game.board[row][col]);
            }

            t1 = t2;
        }
    }

    endwin();

    cout << "You're dead!\nYour score was: " << game.score << "\n";

    return 0;
}
