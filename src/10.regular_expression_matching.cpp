#include <iostream>
#include <string>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <utility>

using namespace std;



struct Pair {
    int first;
    int second;
    Pair(int a, int b) : first(a), second(b) {}
    bool operator==(const Pair& rhs) const { return first == rhs.first && second == rhs.second;} //N.B. Have to define operator== for hash to work
};

template<>
struct std::hash<Pair>
{
    std::size_t operator() (const Pair& v) const {
    return std::hash<int>()(v.first) ^ hash<int>()(v.second) << 1;
    }
};


bool dfs(int i, int j, std::unordered_map<Pair, bool>& cache, std::string& s, std::string& p) {
    if (auto sear = cache.find(Pair(i, j)); sear != cache.end()) {
        return cache[Pair(i, j)];
    }

    if (i >= s.size() && j >= p.size()) {
        return true;    
    }

    if (j >= p.size()) {
        return false;
    }

    auto match = i < s.size() && (s[i] == p[j] || p[j] == '.');

    if (((j + 1) < p.size()) && p[j + 1] == '*') {
        cache[Pair(i, j)] = (dfs(i, j + 2, cache, s, p) || (match && dfs(i + 1, j, cache, s, p)));
        return cache[Pair(i, j)];
    }

    if (match) {
        cache[Pair(i, j)] = dfs(i + 1, j + 1, cache, s, p);
        return cache[Pair(i, j)];
    }

    cache[Pair(i, j)] = false;

    return false;


}



bool isMatch(string s, string p) {
    std::unordered_map<Pair, bool> cache;
//    return true;
    return dfs(0, 0, cache, s, p);
}



int main() {

 

//    std::unordered_map<Pair, std::string> myMap;
//    myMap[Pair(1, 2)] = "hello";
    std::cout << isMatch("aab", ".*b");
//    std::cout << myMap[Pair(1, 2)] << std::endl;

//    auto it = myMap.find(Pair(1, 2));
    //std::cout << *it << std::endl;


//    if (auto search = myMap.find(Pair(1, 2)); search != myMap.end())
//        std::cout << "Found " << std::endl<< search->first.first <<search->first.second <<' ' << search->second << '\n';
//    else
//        std::cout << "Not found\n";

    return 0;
}



#include <string>
#include <unordered_map>
#include <tuple>

using namespace std;

class Solution {
public:
    bool isMatch(string s, string p) {
        unordered_map<string, bool> memo;
        return match(s, p, 0, 0, memo);
    }

private:
    bool match(const string& s, const string& p, int i, int j, unordered_map<string, bool>& memo) {
        string key = to_string(i) + "," + to_string(j);
        if (memo.find(key) != memo.end()) {
            return memo[key];
        }

        if (j == p.size()) {
            return i == s.size();
        }

        bool first_match = (i < s.size() && (p[j] == s[i] || p[j] == '.'));

        if (j + 1 < p.size() && p[j + 1] == '*') {
            bool result = match(s, p, i, j + 2, memo) || (first_match && match(s, p, i + 1, j, memo));
            memo[key] = result;
            return result;
        } else {
            bool result = first_match && match(s, p, i + 1, j + 1, memo);
            memo[key] = result;
            return result;
        }
    }
};



#include <iostream>
#include <string>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <utility>

class Solution {
public:

    bool isMatch(string s, string p) {
     int m = s.size(), n = p.size();
    vector<bool> prev(n + 1, false);
    vector<bool> curr(n + 1, false);
    prev[0] = true;

    for (int j = 1; j <= n; ++j) {
        if (p[j - 1] == '*') {
            prev[j] = prev[j - 2];
        }
    }

    for (int i = 1; i <= m; ++i) {
        curr[0] = false;
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                curr[j] = curr[j - 2];
                if (p[j - 2] == '.' || p[j - 2] == s[i - 1]) {
                    curr[j] = curr[j] || prev[j];
                }
            } else if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = false;
            }
        }
        prev = curr;
    }

    return prev[n];
    }
};
