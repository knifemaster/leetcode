class Solution {
public:
    string longestPalindrome(string s) {
        if (s.empty()) return "";

        int start = 0, max_len = 1;
        int n = s.size();

        for (int i = 0; i < n; ) {
            if ((n-i) <= max_len / 2) break;

            int left = i, right = i;

            while (right < n - 1 && s[right] == s[right + 1] )
                right++;
            
            i = right + 1;

            while (left > 0 && right < n - 1 && s[left - 1] == s[right + 1]) {
                left--;
                right++;
            }

            int curr_len = right - left + 1;
            if (curr_len > max_len) {
                max_len = curr_len;
                start = left;
            }

        }

        return s.substr(start, max_len);        

    }
};
