#include <iostream>
#include <unordered_map>
#include <string_view>
#include <string>

int main() {
    std::unordered_map<std::string, int> myMap = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    std::string_view key = "banana";

    auto it = myMap.find(key); 

    if (it != myMap.end()) {
        std::cout << "Found: " << it->first << " - " << it->second << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}
