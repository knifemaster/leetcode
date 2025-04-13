#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        const int n = nums.size();
        int closest = nums[0] + nums[1] + nums[2]; // Начальное значение
        
        for (int i = 0; i < n - 2; ++i) {
            if (i > 0 && nums[i] == nums[i-1]) continue;
            
            int left = i + 1;
            int right = n - 1;
            
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                if (abs(sum - target) < abs(closest - target)) {
                    closest = sum;
                    if (closest == target) return target; // Ранний выход при точном совпадении
                }
                
                if (sum < target) {
                    do { ++left; } while (left < right && nums[left] == nums[left-1]);
                } else {
                    do { --right; } while (left < right && nums[right] == nums[right+1]);
                }
            }
        }
        return closest;
    }
};




#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int current_sum;
        int n = nums.size() - 1;
        int closest_sum = numeric_limits<int>::max();
        for (int i = 0; i < nums.size(); ++i) {
            
            if (i > 0 && nums[i] == nums[i-1]) {
                continue;
            }
            
            int left = i + 1;
            int right = n;

            while (left < right) {
                current_sum = nums[i] + nums[left] + nums[right];
                
                if (abs(current_sum - target) < abs(closest_sum - target)) {
                    closest_sum = current_sum;
                }
                
                if (current_sum == target) {
                    return current_sum;
                } else if (current_sum < target) {
                    ++left;
                } else {
                    --right;
                }


            }
        }    
        return closest_sum;
    }

int main() {
    vector<int> numbers = {-1, 2, 1, -4};
    int target = 1;

    cout << threeSumClosest(numbers, target) << endl;

    return 0;
}
