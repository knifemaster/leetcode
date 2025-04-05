#include <iostream>
#include <string>
#include <unordered_map>
#include <ranges>
#include <vector>

using namespace std;




int dfs(int i, int j) {
    if (i == 0) {
        return 0;
    }
    std::cout << i << " " <<j << std::endl;
    return dfs(--i, --j);
}


bool isMatch(string s, string p) {
    std::unordered_map<int, int> cache;
    dfs(4, 3);    
    return true;
}


int main() {
    isMatch("helo", "*.");

    return 0;
}
