#include <vector>
#include <iostream>
#include <algorithm>
#include <print>

int removeDuplicates(std::vector<int>& nums) {
   if (nums.empty()) {
        return 0;
    }

    int slow = 1;

    for (int fast = 1; fast < nums.size(); ++fast) {
        if (nums[fast] != nums[slow - 1]) {
            nums[slow] = nums[fast];
            slow++;
        }
    }


    return slow;

}

int main() {
    std::vector<int> numbers = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    std::print("{}", numbers);
    removeDuplicates(numbers);

    std::print("{}", numbers);


    return 0;

}
