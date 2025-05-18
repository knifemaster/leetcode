#include <ranges>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <print>

using namespace std;


vector<string> letterCombinations(string digits) {
    vector<string> buttons = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"} ;
    vector<string> combinations;
    unordered_map<int, string> combinationes;
    
    namespace rv = std::ranges::views;

    int length = digits.size();

    std::cout << length << std::endl;

    vector<int> button_numbers;

    for (auto& digit : digits) {
        auto digit_int = digit - '0';
        button_numbers.push_back(digit_int);
        std::print("{}", digit_int);
    }

    if (length == 2) {
        for (auto&& [a, b] : rv::cartesian_product(buttons[button_numbers[0]], buttons[button_numbers[1]])) {
            std::string s;
            std::print(" {}{}", a, b);
            s.push_back(a);
            s.push_back(b);

            combinations.push_back(s);

        }
    }

    if (length == 3) {
        for (auto&& [a, b, c] : rv::cartesian_product(buttons[button_numbers[0]], buttons[button_numbers[1]], buttons[button_numbers[2]])) {
            std::string s;
            s.push_back(a);
            s.push_back(b);
            s.push_back(c);

            combinations.push_back(s);
            std::print("{} {} {}", a, b, c);
        }
    }

    std::print("{}", combinations);
    return combinations;        

}


int main() {
    string digits = "23";

    auto combinations = letterCombinations(digits);
    


    return 0;

}
