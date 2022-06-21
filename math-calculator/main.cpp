#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

map<char, int> symbols = {{'+', 2}, {'-', 2}, {'*', 1}, {'/', 1}};

bool checkQuery(string query)
{
    for (auto c:query) {
        // Check if the current char is a symbol
        bool isSymbol = 0;
        for (auto i:symbols) {
            if (c == i.first) {
                isSymbol = 1;
                break;
            }
        }

        // Check if the current char is a number
        if ((c >= '0') && (c <= '9'))
            continue;
        // Check if the current char is a space
        else if (c == ' ')
            continue;
        else if (isSymbol)
            continue;
        else
            return 0;
    }
    return 1;
}

string processQuery(string query)
{
    // First let's divide the query into chunks separated by spaces
    int query_length = query.length();

    vector<string> chunks;
    int last_index, cur_index;
    for (last_index = -1, cur_index = 0; cur_index < query_length; cur_index++) {
        if (query[cur_index] == ' ') {
            if (cur_index - last_index > 1) {
                string current_chunk = query.substr(last_index + 1, cur_index - last_index);
                chunks.push_back(current_chunk);
            }

            last_index = cur_index;
        }
    }

    if (query_length - last_index > 1) {
        string current_chunk = query.substr(last_index, query_length - last_index);
        chunks.push_back(current_chunk);
    }

    vector<double> numbers;
    vector<pair<pair<int, int>, char>> operators;

    int symbol_index = 0;
    bool is_last_chunk_number = 0;
    for (auto chunk:chunks) {
        bool is_number = 1;
        try {
            double current_num = stoi(chunk);
            numbers.push_back(current_num);
        } catch (...) {
            operators.push_back({{symbols[chunk[0]], symbol_index}, chunk[0]});
            symbol_index++;
            is_number = 0;
        }

        if (is_number == is_last_chunk_number)
            return "Invalid Query!";
        is_last_chunk_number = is_number;
    }

    if (numbers.size() != operators.size() + 1)
        return "Invalid Query!";

    sort(operators.begin(), operators.end());

    vector<double> ordered_number;
    for (auto i:operators)
        ordered_number.push_back(numbers[i.first.second]);
    ordered_number.push_back(numbers[operators.back().first.second + 1]);

    // Now let's begin calculating
    int operation_count = operators.size();
    double result = ordered_number[0];
    for (int i = 0; i < operation_count; i++) {
        switch (operators[i].second) {
            case '+':
                result += ordered_number[i + 1];
                break;
            case '-':
                result -= ordered_number[i + 1];
                break;
            case '*':
                result *= ordered_number[i + 1];
                break;
            case '/':
                result /= ordered_number[i + 1];
                break;
            default:
                break;
        }
    }

    return to_string(result);
}

int main()
{
    string query;
    getline(cin, query);

    bool is_query_valid = checkQuery(query);
    if (!is_query_valid) {
        cout << "Your query contains unrecognized character(s)! Please check it again!\n";
        exit(-1);
    }

    string result = processQuery(query);
    cout << result << "\n";

    return 0;
}
