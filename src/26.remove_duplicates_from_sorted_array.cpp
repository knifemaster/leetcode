#include <vector>
#include <iostream>
#include <algorithm>
#include <print>

int removeDuplicates(std::vector<int>& nums) {
   if (nums.empty()) {
        return 0;
    }

    int left = 1;

    for (int index = 1; index < nums.size(); ++index) {
        if (nums[index] != nums[left - 1]) {
            nums[left] = nums[index];
            left++;
        }
    }


    return left;

}

int main() {
    std::vector<int> numbers = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    std::print("{}", numbers);
    removeDuplicates(numbers);

    std::print("{}", numbers);


    return 0;

}
