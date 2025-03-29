#include <unordered_map>
#include <vector>

class Solution {
    public:
        vector<int> twoSum(vector<int>& nums, int target) {
            unordered_map<int, int> sums_map;
            int diff;
            for (int i = 0; i < nums.size(); ++i) {
                diff = target - nums[i];
                if (sums_map.find(diff) != sums_map.end()) {
                    return {sums_map[diff], i};    
                }
                sums_map[nums[i]] = i;
            }
            return {};        
        }

};
