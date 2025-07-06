#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <print>




void backtrack(std::vector<int>& candidates, int target, int start, std::vector<int>& current, std::vector<std::vector<int>>& result) {

    if (target == 0) {
        result.push_back(current);
        return;
    }

    for (int i = start; i < candidates.size(); ++i) {
        if (i > start && candidates[i] == candidates[i - 1]) {
            continue;
        }
        
        if (candidates[i] > target) {
            break;
        }

        current.push_back(candidates[i]);

        backtrack(candidates, target - candidates[i], i + 1, current, result);

        current.pop_back();

    }



}


std::vector<std::vector<int>> combinationSum2(std::vector<int>& candidates, int target) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    sort(candidates.begin(), candidates.end());

    backtrack(candidates, target, 0, current, result);
    return result;
}


int main() {

    std::vector<int> candidates = {10, 1, 2, 7, 6, 1, 5};
    int target = 8;

    auto combinations = combinationSum2(candidates, target);


    std::print("{}", combinations);



    return 0;
}
