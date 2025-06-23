#include <iostream>
#include <vector>
#include <algorithm>




class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        auto it = std::lower_bound(nums.begin(), nums.end(), target);
        return std::distance(nums.begin(), it);
    }
};


int main() {

    std::vector<int> numbers = { 1, 3, 5, 6 };
    int target = 4;

    std::cout << searchInsert(numbers, target);

    return 0;
}
