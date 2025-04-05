#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>


class Solution {
public:
  
    int myAtoi(string s) {
        int i = 0;
        int sign = 1;
        long result = 0;
    
        while (i < s.size() && s[i] == ' ') {
            i++;
        }
    
        if (i < s.size() && (s[i] == '-' || s[i] == '+')) {
            sign = (s[i] == '-') ? -1 : 1;
            i++;
        }
    
        bool hasDigits = false;
        while (i < s.size() && isdigit(s[i])) {
            hasDigits = true;
            if (s[i] == '0' && result == 0) {
                i++;
                continue;
            }
        
        result = result * 10 + (s[i] - '0');
        
        if (result * sign > INT_MAX) {
            return INT_MAX;
        }
        if (result * sign < INT_MIN) {
            return INT_MIN;
        }
        
        i++;
    }
    if (!hasDigits) {
        return 0;
    }
    
    return (int)(result * sign);
    }
};

/*
std::string trim_left(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    return std::string(it, s.end());
}

std::string trim_right(const std::string& s) {
    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != s.begin() && isspace(*rit))
        rit++;
    return std::string(s.begin(), rit.base());
}


std::string trim(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}

std::string get_string_without_sign(const std::string& s) {
    std::string::const_iterator it = s.begin();
    if (*it == '+' || *it == '-' ) {
        it++;
        return std::string(it, s.end());
    }
    return s;    
}

std::string get_string_without_leading_zeroes(const std::string& s) {
    std::string::const_iterator it = s.begin();
    if (*it == '0') {
        it++;
        return std::string(it, s.end());
    }
    return s;

}

std::string get_valid_string(const std::string& s) {
    std::string_view str(s);
    std::string_view::const_iterator it = str.begin();
    bool is_negative = false;

    while (!isdigit(*it)) {
        if (*it == ' ')
            it++;
        if (*it == '+')
            it++;
        if (*it == '0')
            it++;
        if (* it == '-') {
            it++;
            is_negative = true;
        }
        
    }

    if (is_negative) {
        return "-" + std::string(it, str.end());
    }
    return std::string(it, str.end());

}

int main() {
    std::string str1 = "               +5454     ";
    std::string string_number = "  123";

    std::cout << trim(string_number) << std::endl;
    std::cout << get_string_without_leading_zeroes(get_string_without_sign(trim_left(str1))) << std::endl;
    std::cout << get_valid_string(str1) << std::endl;
    return 0;
}
*/
