class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
    
        vector<vector<int>> result;
        const int n = nums.size();
        if (n < 3) return result;
        
        sort(nums.begin(), nums.end());
        result.reserve(n); // Предварительное выделение памяти
        
        for (int i = 0; i < n - 2; ) {
            const int target = -nums[i];
            int left = i + 1;
            int right = n - 1;
            
            while (left < right) {
                const int sum = nums[left] + nums[right];
                
                if (sum < target) {
                    ++left;
                } else if (sum > target) {
                    --right;
                } else {
                    result.emplace_back(initializer_list<int>{nums[i], nums[left], nums[right]});
                    
                    // Пропуск дубликатов
                    while (left < right && nums[left] == nums[left + 1]) ++left;
                    while (left < right && nums[right] == nums[right - 1]) --right;
                    ++left;
                    --right;
                }
            }
            
            // Пропуск дубликатов для nums[i]
            while (i < n - 2 && nums[i] == nums[i + 1]) ++i;
            ++i;
        }
        
        return result;
    }
};





#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    
    std::sort(nums.begin(), nums.end());
    
    vector<vector<int>> res;
    int left = 0;
    int right = nums.size() - 1;
    int sum = 0;

    for (int i = 0; i < nums.size(); i++) {
        if (i && nums[i] == nums[i - 1]) {
            continue;
        }    
        left = i + 1;
        right = nums.size() - 1;

        while (left < right) {
            sum = nums[i] + nums[left] + nums[right];
            
            if (sum < 0) {
                left++;
            } 
            else if (sum > 0) {
                right--;
            } else {
                vector<int> three;
                three.emplace_back(nums[i]);
                three.emplace_back(nums[left]);
                three.emplace_back(nums[right]);
                res.push_back(three);
                left++;
                while ((nums[left] == nums[left-1]) && left < right) {
                    left++;
                }
            }

        }
    }

    return res;

}


int main() {
    vector<int> numbers = {-1, 0, 1, 2, -1, -4};
    auto vecs = threeSum(numbers);
    
    for (const auto& vec : vecs) {
        for (const auto& num : vec) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    
    }

    return 0;
}
