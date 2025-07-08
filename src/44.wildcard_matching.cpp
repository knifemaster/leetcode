#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool isMatch(string s, string p) {
    int m = s.size(), n = p.size();
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
    dp[0][0] = true;

  
    for (int j = 1; j <= n; ++j) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            } else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    return dp[m][n];
}

int main() {
    string s1 = "aa";
    string p1 = "a";
    cout << "Example 1: " << isMatch(s1, p1) << endl;

    string s2 = "aa";
    string p2 = "*";
    cout << "Example 2: " << isMatch(s2, p2) << endl;

    string s3 = "cb";
    string p3 = "?a";
    cout << "Example 3: " << isMatch(s3, p3) << endl;

    return 0;
}
