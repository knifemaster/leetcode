class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    result.reserve(4);
    std::vector<int> quad;

    if (nums.size() < 4) {
        return result;
    }

    std::function<void(int, int, long)> kSum;

    kSum = [&quad, &nums, &kSum, &result](int k, int start, long target) {
        if (k != 2) {
            for (int i = start; i < nums.size() - k + 1; i++) {
                if (i > start && nums[i] == nums[i - 1]) {
                    continue;
                }
                quad.push_back(nums[i]);
                kSum(k - 1, i + 1, target - nums[i]);
                quad.pop_back();
            }
            return;
        }

        int left = start;
        int right = nums.size() - 1;

        while (left < right) {
            long sum = static_cast<long>(nums[left]) + nums[right];
            if (sum < target) {
                left++;
            } else if (sum > target) {
                right--;
            } else {
                // Found a pair
                std::vector<int> temp = quad;
                temp.reserve(4);
                temp.push_back(nums[left]);
                temp.push_back(nums[right]);
                result.push_back(temp);
                
                left++;
                while (left < right && nums[left] == nums[left - 1]) {
                    left++;
                }
            }
        }
    };

    kSum(4, 0, target);
    return result;    
    }
};

