#include <iostream>
#include <vector>
#include <algorithm>
#include <print>

std::vector<std::vector<int>> permute(std::vector<int>& nums) {


    std::vector<std::vector<int>> result;

    std::vector<int> first_vector = nums;

    bool first_try = false;

    do {
        if (first_try) {
            result.push_back(first_vector);
        }
        first_try = true;
    }

    while (std::prev_permutation(first_vector.begin(), first_vector.end()));



    do {
        result.push_back(nums);
        std::print("{}", nums);
    }
    while (std::next_permutation(nums.begin(), nums.end()));


    return result;

}


int main() {

    std::vector<int> nums = {1, 2, 3};

    std::cout << std::endl;

    auto permuted_value = permute(nums);

    std::cout << std::endl;

    std::print("{}", permuted_value);


    return 0;
}
