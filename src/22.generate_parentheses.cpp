#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <stack>


std::string is_valid(std::string& s) {
    std::stack<char> queue;
    int left_paren_count = 0;
    int right_paren_count = 0;
    
    if (s.size() % 2 != 0) {
        return "";
    }
    bool is_occur = false;
    for (const auto& parentheses : s) {
        if (parentheses == '(') {    
            queue.push(parentheses);
            left_paren_count++;
        } else {
            right_paren_count++;
            is_occur = true;
            
            if (queue.empty()) {
                return "";
            }
            char close_paren = queue.top();
            if (parentheses == ')') {
                if (close_paren == '(') {
                    queue.pop();
                } else {
                    return "";
                }
            }
        }


    }

    if (left_paren_count != right_paren_count) {
        return "";
    }

    if (is_occur) 
        return s;
    return "";
    }
    



static std::vector<std::string> parentheses = {
            "",
            "()",
            "()()",
            "()()()",
            "()()()()",
            "()()()()()",
            "()()()()()()",
            "()()()()()()()",
            "()()()()()()()()"
};



std::vector<std::string> generateParenthesis(int n) {

    std::vector<std::string> result;
    std::string temp = parentheses[n];

    std::prev_permutation(temp.begin(), temp.end());

    
    do {
        std::cout << temp << '\n';
        if (is_valid(temp) != "") {
            result.push_back(temp);
        }
    }

    while (std::prev_permutation(temp.begin(), temp.end()));

    std::string second_part = parentheses[n];


    do {
        if (is_valid(second_part) != "") {
            std::cout << second_part << '\n';
            result.push_back(second_part);
        }
    }
    while (std::next_permutation(second_part.begin(), second_part.end()));



        
    return result;

}


int main() {
    //for (int i = 0; i < 8; i++) {
    //generateParenthesis(i);
    //}
    

    std::vector<std::string> vec_parens = generateParenthesis(4);

    std::cout << "*******************************" << std::endl;
    for (std::string& paren : vec_parens) {
    
        std::string valid = is_valid(paren);
   //     std::cout << valid << std::endl;
        if (valid != "") {
            std::cout << valid << std::endl;
        }
    }



    return 0;
}
