#include <unordered_set>
#include <unordered_map>


class Solution {
public:
    int lengthOfLongestSubstring(string s) {   
        vector<int> lastIndex(128, -1);
        int maxLen = 0;
        int left = 0;
        
        for (int right = 0; right < s.size(); ++right) {
            char c = s[right];
            if (lastIndex[c] >= left) {
                left = lastIndex[c] + 1;
            }
            lastIndex[c] = right;
            maxLen = max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
};

#include <unordered_set>
#include <algorithm>

class Solution {
public:
    int lengthOfLongestSubstring(std::string s) {
        std::unordered_set<char> charSet;
        int left = 0;
        int maxLength = 0;
        
        for (int right = 0; right < s.size(); ++right) {
            while (charSet.find(s[right]) != charSet.end()) {
                charSet.erase(s[left]);
                ++left;
            }
            charSet.insert(s[right]);
            maxLength = std::max(maxLength, right - left + 1);
        }
        
        return maxLength;
    }
};
