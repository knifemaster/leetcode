#include <vector>
#include <iostream>
#include <algorithm>
#include <print>


class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
    const auto range = std::equal_range(nums.begin(), nums.end(), target);
    std::vector<int> result;
    result.reserve(2);

    if (std::distance(range.first, range.second) == 0) {
        return {-1, -1};
    }

    result.push_back(std::distance(nums.begin(), range.first));
    result.push_back(std::distance(nums.begin(), range.second) - 1);

    return result;    
    }
};


int main() {

    std::vector<int> numbers = { 5, 7, 7, 8,  8, 10 };

    int target = 6;

    std::print("{}", searchRange(numbers, target));


    return 0;

}
