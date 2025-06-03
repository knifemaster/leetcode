
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

class Solution {
public:

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

    vector<string> generateParenthesis(int n) {

        if (n == 1) {
            return {"()"};

        }
    
        std::vector<std::string> result;
        result.reserve(n*n*3);
        std::string temp = parentheses[n];

        std::prev_permutation(temp.begin(), temp.end());

        
        do {
            //std::cout << temp << '\n';
            if (is_valid(temp) != "") {
                result.push_back(temp);
            }
        }

        while (std::prev_permutation(temp.begin(), temp.end()));

        std::string second_part = parentheses[n];


        do {
            if (is_valid(second_part) != "") {
                //std::cout << second_part << '\n';
                result.push_back(second_part);
            }
        }
        while (std::next_permutation(second_part.begin(), second_part.end()));
        
        return result;
    }
};
