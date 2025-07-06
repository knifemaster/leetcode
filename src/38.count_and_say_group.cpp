
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <ranges>
#include <algorithm>
#include <sstream>

int main() {

//    std::string str = "123456789123";
    std::vector<int> str = {1,2,3,5,6,1,2,3,4,5,6, 3,4,5, 1, 2, 3, 11,12,13};
    std::stringstream result;

    auto grouped = str | std::views::chunk_by([](int a, int b) {
        return a == b - 1;
    });

    for (auto group : grouped) {
        char c = group.front();

        size_t count = std::ranges::distance(group);


        for (const auto& val : group) {

            std::cout << val << " ";
        }
        std::cout << std::endl;

        std::cout << c << std::endl;

        result << count << c;
        //std::cout << result.str() << std::endl;
    }

    //std::cout << result.str() << std::endl;


}

