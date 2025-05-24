class Solution {
public:
    bool isValid(string s) {
    std::stack<char> queue;
    int left_paren_count = 0;
    int right_paren_count = 0;
    
    if (s.size() % 2 != 0) {
        return false;
    }
    bool is_occur = false;
    for (const auto& parentheses : s) {
        if (parentheses == '(' || parentheses == '[' || parentheses == '{') {    
            queue.push(parentheses);
            left_paren_count++;
        } else {
            right_paren_count++;
            is_occur = true;
            
            if (queue.empty()) {
                return false;
            }
            char close_paren = queue.top();
            if (parentheses == ')') {
                if (close_paren == '(') {
                    queue.pop();
                } else {
                    return false;
                }
            }
            if (parentheses == ']') {
                if (close_paren == '[') {
                    queue.pop();
                } else {
                    return false;
                }
            }
            if (parentheses == '}') {
                if (close_paren == '{') {
                    queue.pop();
                } else {
                    return false;
                }
            }
        }


    }

    if (left_paren_count != right_paren_count) {
        return false;
    }

    if (is_occur) 
        return true;
    return false;
    }
};
