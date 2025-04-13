#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int result;
        int diff;
        int sum;
        int n = nums.size() - 1;
        int min_diff = 1000;
        for (int i = 0; i < nums.size(); ++i) {
            int left = i + 1;
            int right = n;

            while (left < right) {
                sum = nums[i] + nums[left] + nums[right];
                diff = target - sum;
                min_diff = min(diff, min_diff);
                cout << "diff" << diff << endl;
                cout << "min" << min_diff << endl;
                if (sum < 0) {
                    left++;
                } else if (sum > 0) {
                    right--;
                } else {
                   min_diff = min(diff, min_diff);
     
                    left++;
                    while((nums[left] == nums[left-1]) && left < right) {
                        left++;
                    }
                }

            }
        }    
        return min_diff;
    }

int main() {
    vector<int> numbers = {-1, 2, 1, -4};
    int target = 1;

    cout << threeSumClosest(numbers, target) << endl;

    return 0;
}
