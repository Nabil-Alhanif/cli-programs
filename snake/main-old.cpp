#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <vector>
using namespace std;

enum direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
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

class Segment {
    public:
        int row;
        int col;
        direction dir;
        Segment* next;

        Segment()
        {
            this->next = nullptr;
        }

        Segment(int r, int c, direction d)
        {
            this->row = r;
            this->col = c;
            this->dir = d;
            this->next = nullptr;
        }
};

struct Snake {
    public:
        int score;
        int food_value;

        int max_row;
        int max_col;

        Segment* head;
        Segment* tail;

        vector<vector<char>> board;

        Snake(int width, int height, int food_val = 10)
        {
            this->score = 0;
            this->food_value = food_val;

            this->max_row = height;
            this->max_col = width;

            this->board.resize(this->max_row);
            for (auto &i:this->board)
                i.resize(this->max_col, EMPTY);

            int start_row = height / 2;
            int start_col = (width / 2) - 1;

            this->head = new Segment(start_row, start_col, LEFT);
            Segment* mid = new Segment(start_row, start_col + 1, LEFT);
            this->tail = new Segment(start_row, start_col + 2, LEFT);

            this->head->next = mid;
            mid->next = this->tail;

            for (int i = 0; i < 3; i++)
                this->board[start_row][start_col + i] = LEFT_C;

            generateFood();
        }

        bool move()
        {
            bool is_food_eaten = 0;
            bool has_hit_self = 0;

            int tail_row, tail_col;
            tail_row = this->tail->row;
            tail_col = this->tail->col;

            direction last_move = this->head->dir, tmp;
            for (Segment* cur = this->head; ; cur = cur->next) {
                this->board[cur->row][cur->col] = EMPTY;

                char ch;
                switch (cur->dir) {
                    case (UP):
                        cur->row--;
                        ch = UP_C;
                        break;
                    case (DOWN):
                        cur->row++;
                        ch = DOWN_C;
                        break;
                    case (LEFT):
                        cur->col--;
                        ch = LEFT_C;
                        break;
                    case (RIGHT):
                        cur->col++;
                        ch = RIGHT_C;
                        break;
                }

                // Normalize current segment position
                cur->row = (cur->row + max_row) % max_row;
                cur->col = (cur->col + max_col) % max_col;

                // Check if after current movement we will eat the food
                if (this->board[cur->row][cur->col] == FOOD)
                    is_food_eaten = 1;
                else if (this->board[cur->row][cur->col] != EMPTY) {
                    has_hit_self = 1;
                    break;
                }

                this->board[cur->row][cur->col] = ch;

                tmp = cur->dir;
                cur->dir = last_move;
                last_move = tmp;

                if (cur->next == nullptr)
                    break;
            }

            if (is_food_eaten) {
                this->score += this->food_value;
                this->generateFood();

                // Add new segment
                this->tail->next = new Segment(tail_row, tail_col, last_move);
                this->tail = this->tail->next;
            }

            if (has_hit_self)
                return 0;

            return 1;
        }

        void generateFood()
        {
            int row, col;

            do {
                row = rand() % this->max_row;
                col = rand() % this->max_col;
            } while(this->board[row][col] != EMPTY);

            this->board[row][col] = FOOD;
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

    Snake snake(max_col - 2, max_row - 3);

    bool is_game_running = 1;

    while (is_game_running) {
        ch = getch();
        if ((ch == KEY_UP) && ((snake.head->dir == LEFT) || (snake.head->dir == RIGHT)))
            snake.head->dir = UP;
        else if ((ch == KEY_DOWN) && ((snake.head->dir == LEFT) || (snake.head->dir == RIGHT)))
            snake.head->dir = DOWN;
        else if ((ch == KEY_LEFT) && ((snake.head->dir == UP) || (snake.head->dir == DOWN)))
            snake.head->dir = LEFT;
        else if ((ch == KEY_RIGHT) && ((snake.head->dir == UP) || (snake.head->dir == DOWN)))
            snake.head->dir = RIGHT;

        t2 = chrono::steady_clock::now();
        chrono::duration<float> elapsed_time = t2 - t1;

        // Do calculation every .1 seconds
        if (elapsed_time.count() > 0.1f) {
            is_game_running = snake.move();

            erase();
            mvprintw(0, 0, "Current score: %d", snake.score);

            // Draw border
            for (int i = 0; i < max_col; i++) {
                mvaddch(1, i, BORDER); // Upper
                mvaddch(max_row - 1, i, BORDER); // Lower
            }

            for (int i = 1; i < max_row; i++) {
                mvaddch(i, 0, BORDER); // Left
                mvaddch(i, max_col - 1, BORDER); // Right
            }

            for (int row = 0; row < snake.max_row; row++) {
                for (int col = 0; col < snake.max_col; col++)
                    mvaddch(row + 2, col + 1, snake.board[row][col]);
            }

            t1 = t2;
        }
    }

    endwin();

    cout << "You're dead!\nYour score was: " << snake.score << "\n";

    return 0;
}
