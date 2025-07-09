class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
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
        //    std::print("{}", nums);
        }
        while (std::next_permutation(nums.begin(), nums.end()));


        return result;   
    }
};
