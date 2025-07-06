#include <vector>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <print>




void backtrack(std::vector<int>& candidates, int target, int start, std::vector<int>& current, std::vector<std::vector<int>>& result) {

    if (target == 0) {
        result.push_back(current);
        return;
    }

    for (int i = start; i < candidates.size(); i++) {
        if (candidates[i] > target) {
        //    std::cout << "*************when target < our candidates[i]"<<std::endl;
        //    std::cout << "candidates[] " << candidates[i] << " " << i << std::endl; 
            continue;
        }

        //std::cout << "index: " << i << std::endl;

        current.push_back(candidates[i]);
  
        //std::println("current is [[[ {} ]]]", current);
        //std::println("result is [[[ {} ]]]", result);
        backtrack(candidates, target - candidates[i], i, current, result);

        //std::cout << "pop_back operation from current" << std::endl;
        current.pop_back();
    }

}


std::vector<std::vector<int>> combinationSum(std::vector<int>& candidates, int target) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    std::sort(candidates.begin(), candidates.end()); 
    backtrack(candidates, target, 0, current, result);
    return result;
}



int main() {
    std::vector<int> candidates = {2,3,5};
    int target = 8;

    auto res = combinationSum(candidates, target);

    for (const auto& combination : res) {
        std::print("{}", combination);
        std::cout << std::endl;
    }

    return 0;

}
