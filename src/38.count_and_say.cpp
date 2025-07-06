#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <ranges>
#include <algorithm>
#include <sstream>


std::string helper(int n, std::vector<std::string>& dp) {

    if (dp[n] != "") {
        return dp[n];
    }

    if (n  == 1) {
        dp[1] = "1";
        return "1";
    }


    std::string str = helper(n-1, dp);
    std::stringstream result;

    auto grouped = str | std::views::chunk_by([](char a, char b) {
        return a == b;
    });

    for (auto group : grouped) {
        char c = group.front();

        size_t count = std::ranges::distance(group);

        result << count << c;
    }

    std::string encoded = result.str();

    std::cout << encoded << std::endl;

    dp[n] = result.str();

    return dp[n];


}


std::string countAndSay(int n) {
    std::vector<std::string> dp(n + 1, "");
    return helper(n, dp);

}



int main() {
    //int n = 4;
    //std::vector<std::string> dp(n + 1, "");
    std::cout << countAndSay(10) << std::endl;

    

    return 0;
}
