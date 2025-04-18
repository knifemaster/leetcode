#include <iostream>
#include <string>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <utility>

using namespace std;


bool dfs(int i, int j) {
    if (auto search = cache.find(Pair(i, j); search != cache.end())) {
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
        cache[Pair(i, j)] = (dfs(i, j + 2) || (match && dfs(i + 1, j)));
        return cache[Pair(i, j)];
    }

    if (match) {
        cache[Pair(i, j)] = dfs(i + 1, j + 1);
        return cache[Pair(i, j)];
    }

    cache[Pair(i, j)] = false;

    return false;


}



bool isMatch(string s, string p) {
    std::unordered_map<Pair, bool> cache;

    return dfs(0, 0, cache, s, p);
}


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

int main() {

 

    std::unordered_map<Pair, std::string> myMap;
    myMap[Pair(1, 2)] = "hello";
    isMatch("aab", ".*b");
    std::cout << myMap[Pair(1, 2)] << std::endl;

    auto it = myMap.find(Pair(1, 2));
    //std::cout << *it << std::endl;


    if (auto search = myMap.find(Pair(1, 2)); search != myMap.end())
        std::cout << "Found " << std::endl<< search->first.first <<search->first.second <<' ' << search->second << '\n';
    else
        std::cout << "Not found\n";

    return 0;
}

