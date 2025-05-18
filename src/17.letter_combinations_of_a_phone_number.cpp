#include <memory_resource>
#include <ranges>
#include <print>

namespace rv = std::ranges::views;
using namespace std;


class Solution {
public:
    vector<string> letterCombinations(string digits) {
    vector<string> buttons = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"} ;
    vector<string> combinations;
    unordered_map<int, string> combinationes;
    
    namespace rv = std::ranges::views;

    int length = digits.size();

    std::cout << length << std::endl;

    vector<int> button_numbers;

    if (length == 1) {
        auto index = digits[0] - '0';
        for (auto letter : buttons[index]) {
            std::string s;
            s.push_back(letter);
            combinations.push_back(s);
        }
    }

    for (auto& digit : digits) {
        auto digit_int = digit - '0';
        button_numbers.push_back(digit_int);
    }

    if (length == 2) {
        for (auto&& [a, b] : rv::cartesian_product(buttons[button_numbers[0]], buttons[button_numbers[1]])) {
            std::string s;
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
        }
    }

    if (length == 4) {
        for (auto&& [a, b, c, d] : rv::cartesian_product(buttons[button_numbers[0]], buttons[button_numbers[1]], buttons[button_numbers[2]], buttons[button_numbers[3]])) {
            std::string s;
            s.push_back(a);
            s.push_back(b);
            s.push_back(c);
            s.push_back(d);

            combinations.push_back(s);
        }

    }
    return combinations;        

}
   
    
};
