#include <cstring>
#include <iostream>
#include <string>
using namespace std;

char board[3][3] = {{'1', '2', '3'},
                    {'4', '5', '6'},
                    {'7', '8', '9'}};

void drawBoard()
{
    string separator = "-------";
    cout << separator << "\n";
    for (int i = 0; i < 3; i++) {
        cout << '|';
        for (int j = 0; j < 3; j++)
            cout << board[i][j] << '|';
        cout << "\n";
        cout << separator << "\n";
    }
}

bool checkWin()
{
    bool is_win = 0;

    // Check horizontally
    for (int col = 0; col < 3; col++) {
        if ((board[col][0] != ' ') && ((board[col][0] == board[col][1]) && (board[col][0] == board[col][2])))
            is_win = 1;
    }

    // Check vertically
    for (int row = 0; row < 3; row++) {
        if ((board[0][row] != ' ') && ((board[0][row] == board[1][row]) && (board[0][row] == board[2][row])))
            is_win = 1;
    }

    // Check diagonally
    if ((board[0][0] != ' ') && ((board[0][0] == board[1][1]) && (board[0][0] == board[2][2])))
        is_win = 1;

    if ((board[0][2] != ' ') && ((board[0][2] == board[1][1]) && (board[0][2] == board[2][0])))
        is_win = 1;

    return is_win;
}

int main()
{
    drawBoard();

    memset(board, ' ', sizeof(board));

    char current_player = 'X';
    for (int i = 0; i < 9; i++) {
        bool is_pick_empty = 0;
        int picked_slot;

        do {
            cout << "Player " << current_player << ", pick an empty slot from 1 - 9!\n";
            cin >> picked_slot;
            picked_slot--;

            if (board[picked_slot / 3][picked_slot % 3] == ' ')
                is_pick_empty = 1;
            else cout << "Player " << current_player << ", the slot " << picked_slot + 1 << " is not empty!\nPlease pick another one!\n";

        } while (!is_pick_empty);

        board[picked_slot / 3][picked_slot % 3] = current_player;

        drawBoard();

        if (checkWin()) {
            cout << "Congratulations Player " << current_player << ", you've won!\n";
            break;
        }

        if (i == 8) {
            cout << "It's a tie!\n";
            break;
        }

        // Switch the player
        if (current_player == 'X')
            current_player = 'O';
        else current_player = 'X';
    }

    return 0;
}
