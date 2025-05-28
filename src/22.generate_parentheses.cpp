#include <iostream>
#include <string>
#include <ranges>
#include <vector>


std::vector<std::string> generateParenthesis(int n) {
    std::vector<std::string> result;
    std::string left_parens = "()()()";
    std::string right_parens = "()()()";
    std::string mid_parens = "()()()";
    std::string last_parens = "()()()";

    for (const auto& [a, b, c, d] : std::ranges::views::cartesian_product(left_parens, right_parens, mid_parens, last_parens)) {
        std::cout << a << " " << b << " " << c  << " "<< d << std::endl;
    }

    return result;

}

std::string parentheses = "()()()()";
int size = parentheses.size();


std::string backtrack(int n) {
    if (n > size) {
        return ""; 
    }

    return backtrack(n--);

}



int main() {
    generateParenthesis(4);
    
    return 0;
}
