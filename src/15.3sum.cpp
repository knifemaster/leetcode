#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    int result = 0;
    vector<vector<int>> res;
    for (int i = 0; i < nums.size(); i++) {
        for (int j = 0; j < nums.size(); j++) {
            for (int k = 0; k < nums.size(); k++) {
                if (i != j && i != k && j != k ) {
                    if(nums[i] + nums[j] + nums[k] == 0) {
                        std::cout << i << " " << j <<" " << k << std::endl;
                    }    
                }
            }
        }
    }
    return res;    

}


int main() {
    vector<int> numbers = {-1, 0, 1, 2, -1, -4};
    threeSum(numbers);
    

    return 0;
}
