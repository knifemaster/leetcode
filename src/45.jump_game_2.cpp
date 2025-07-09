#include <iostream>
#include <vector>
#include <algorithm>
#include <print>



int jump(std::vector<int>& nums) {

    int jump_count = 0;
    int left = 0; 
    int right = 0;

    int last_index = nums.size() - 1;

    while (right < last_index) {
        int farthest = 0;

        std::cout << "left: " << left << std::endl;
        std::cout << "right: " << right << std::endl;
        

        for (int i = left; i < right + 1; i++) {
            std::cout << "farthest: " << farthest << " i: " << i << " i+nums[i]: " << i + nums[i] << std::endl;
            farthest = std::max(farthest, i + nums[i]);
        }

        std::cout << "jump count" << jump_count << std::endl;

        left = right + 1;
        right = farthest;
        jump_count += 1;

    }

    return jump_count;
}


int main() {

    std::vector<int> nums = {2, 3, 1, 1, 4};

    std::cout << jump(nums) << std::endl;

    return 0;
}
