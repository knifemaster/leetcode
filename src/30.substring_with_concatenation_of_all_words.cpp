#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <vector>
#include <print>
#include <string_view>


std::vector<int> findSubstring(std::string s, std::vector<std::string>& words) {
    std::vector<int> result;

    return result;

}

int main() {

    std::string s = "barfoofoobarthefoobarman";

    std::vector<std::string> words = {"bar", "foo", "the"};
    std::vector<int> nums = {0, 1, 2};
    std::print("{}", nums);

    std::vector<std::string> vec_words; 

    do {
        //std::next_permutation(nums.begin(), nums.end());
        
        std::string word = words[nums[0]] + words[nums[1]] + words[nums[2]];
        std::cout << word << std::endl;
        vec_words.push_back(word);
        //std::print("{}", nums);
    }

    while (std::next_permutation(nums.begin(), nums.end()));// {
    //    std::print("{}", nums);    
    //}
    
    std::string_view s_view = s;
    
    for (auto& word : vec_words) {
        std::string_view find_el = word;

        auto index = s_view.find(find_el);
        if (index != -1) {
            std::cout << index << std::endl;
        }
        //std::cout << 

    }



    //std::string s
    
    return 0;
}

