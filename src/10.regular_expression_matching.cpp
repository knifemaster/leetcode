#include <iostream>
#include <string>
#include <unordered_map>
#include <ranges>
#include <vector>

using namespace std;


// s = "aab";
// p = ".*b"
// dp = [false, false, false, false]


bool isMatch(string s, string p) {
    size_t string_size = s.size();
    size_t pattern_size = p.size();
    std::vector<bool> dp(pattern_size + 1, false);

    bool is_matched = false;

    for (size_t i = 0; i < string_size; i++) {
        
        bool prev = dp[0];
        dp[0] = i == 0;

        for (size_t j = 0; j < pattern_size; j++) {
            bool temp = dp[j];

            if (p[j - 1] == '*') {
                dp[j] = 
            }

        }
    }

    return true;
}


int main() {
    isMatch("helo", "*.");

    return 0;
}
