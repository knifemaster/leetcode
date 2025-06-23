#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>



std::pair<int, int> binary_search(std::vector<int>& nums) {

    int left = 0;
    int right = nums.size() - 1;
   
    int mid = 0;

    while (left < right) {
        if (nums[left] <= nums[right]) {
            return {nums[right], right};
        } 
        mid = (right + left) / 2;
        
        if (nums[mid] > nums[right]) {
            left = mid;
        }
        else {
            right = mid - 1;
        }
    }
    return {nums[left], left};

}

int binary_search_index(std::vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;

    int mid;

    while(left <= right) {
        mid = (left + right) / 2;

        if (target == nums[mid]) {
            return mid;
        }

        if (nums[left] <= nums[mid]) {
            if (target > nums[mid] || target < nums[left]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }    
        } else {
            if (target < nums[mid] || target > nums[right]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
    }

    return -1;

}



int search(std::vector<int>& nums, int target) {
    
    auto peak = binary_search(nums);

    bool is_finded = false;
    int index = -1;
    std::vector<int>::iterator it;

    if (target < peak.first) {
        it = std::lower_bound(nums.begin(), nums.begin() + peak.second, target);
    } else {
        it = std::lower_bound(nums.begin() + peak.second, nums.end(), target);
    }

    //it--;

    std::cout << "value " << *it << std::endl;
    std::cout << "index " << std::distance(nums.begin(), it) << std::endl;
    std::cout << "target " << target << std::endl;

    if (*it == target) {
        return std::distance(nums.begin(), it);
    } else {
        return -1;
    }


}

int main() {

    std::vector<int> numbers = { 4, 5, 6, 7, 0, 1, 2 };

    std::cout << search(numbers, 1) << std::endl;
    std::cout << binary_search_index(numbers, 1) << std::endl;
    return 0;
}
