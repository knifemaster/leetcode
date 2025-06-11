#include <print>
#include <iostream>
#include <vector>


int removeElement(std::vector<int>& nums, int val) {

    int left = 0;
    int right = nums.size();

    while (left < right) {
        if (nums[left] == val) {
            nums[left] = nums[right - 1];
            right--;
        } else {
            left++;
        }
    }

    return left;

}


int main() {

    std::vector<int> numbers = {3, 2, 2, 3};
    int val = 3;

    std::println("{}", numbers);

    removeElement(numbers, val);

    std::println("{}", numbers);

    return 0;
}
