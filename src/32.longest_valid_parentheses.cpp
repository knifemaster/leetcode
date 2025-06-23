#include <iostream>
#include <string>
#include <vector>
#include <stack>


int longestValidParentheses(std::string& s) {
    std::stack<char> stack_of_parens;
    int size_of_valid_parentheses = 0;

    int left_paren_count = 0;
    int right_paren_count = 0;
    
    std::stack<int> stack_indices;
    stack_indices.push(-1);

    

    for (int index = 0; index < s.size(); index++) {
        if (s[index] == '(') {
            stack_indices.push(index);
        } else {
            stack_indices.pop();
            if (stack_indices.empty()) {
                stack_indices.push(index);
            } else {
                size_of_valid_parentheses = std::max(size_of_valid_parentheses, index - stack_indices.top());
            }
        }

    }

    return size_of_valid_parentheses;

}


/*
    for (const auto& parentheses : s) {
        if (parentheses == '(') {
            stack_of_parens.push(parentheses);
            left_paren_count++;
            std::cout << "left: " << left_paren_count << std::endl;
        } else {
            right_paren_count++;
            std::cout << "right: " << right_paren_count << std::endl;
            
            char close_paren;

            if (!stack_of_parens.empty()) {
                close_paren = stack_of_parens.top();
            }
            //char close_paren = stack_of_parens.top();
            if (parentheses == ')') {
                if (close_paren == '(') {
                    if (!stack_of_parens.empty()) {
                        stack_of_parens.pop();
                        size_of_valid_parentheses++;
                    }
                    //size_of_valid_parentheses++;
                }               
               if (right_paren_count == left_paren_count) {
                    std::cout << "Hello world" << std::endl;
               } 
            }
            
        }

        //size_of_valid_parentheses = std::max(right_paren_count, left_paren_count);
        //if (left_paren_count == right_paren_count) {
        //    size_of_valid_parentheses++;
        //}
    }

    return size_of_valid_parentheses+1;



}
*/

int main() {

    std::string s = ")()())";
    std::cout << longestValidParentheses(s) << std::endl;

    return 0;
}
