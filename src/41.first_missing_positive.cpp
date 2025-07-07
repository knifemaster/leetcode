#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <print>
#include <algorithm>

int firstMissingPositive(std::vector<int>& nums) {
    int n = nums.size();
    
    for (int i = 0; i < n; ++i) {
        while (nums[i] > 0 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
            std::swap(nums[i], nums[nums[i] - 1]);
            std::println("{}", nums);
        }
    }
    
    for (int i = 0; i < n; ++i) {
        if (nums[i] != i + 1) {
            std::println("{}", i + 1);
            return i + 1;
        }
    }
    
    return n + 1;
}



int main() {
    std::vector<int> numbers = {3,4,-1,1};
    std::print("{}", firstMissingPositive(numbers));

    return 0;
}

