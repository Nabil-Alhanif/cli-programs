#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

char symbols[4] = {'+', '-', '*', '/'};

int main()
{
    srand((unsigned)time(0));

    int num_1 = (rand() % 10) + 1;
    int num_2 = (rand() % 10) + 1;

    int symbol_index = rand() % 4;
    char symbol = symbols[symbol_index];

    if (symbol == '/') {
        if (num_1 % num_2 != 0) {
            symbol_index = rand() % 3;
            symbol = symbols[symbol_index];
        }
    }

    cout << "What is " << num_1 << " " << symbol << " " << num_2 << "?\n";

    int ans;
    switch (symbol) {
        case '+':
            ans = num_1 + num_2;
            break;
        case '-':
            ans = num_1 - num_2;
            break;
        case '*':
            ans = num_1 * num_2;
            break;
        case '/':
            ans = num_1 / num_2;
            break;
        default:
            break;
    }

    int user_answer;
    cin >> user_answer;

    if (user_answer == ans)
        cout << "Congratulations, you've answered correctly!\n";
    else cout << "Your answer is incorrect! The correct answer is " << ans << "!\n";

    return 0;
}
