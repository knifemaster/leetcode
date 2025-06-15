#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <vector>
#include <print>
#include <string_view>
#include <unordered_map>

struct string_hash {
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    size_t operator()(std::string_view str) const { return hash_type{}(str); }
//    size_t operator()(const std::string& str) const { return hash_type{}(str); }
};


std::vector<int> findSubstring(std::string s, std::vector<std::string>& words) {
    std::vector<int> result;

    std::unordered_map<std::string, int, string_hash, std::equal_to<>> frequency_words;

    for (auto& word : words) {
        frequency_words.insert({word, 1});
    }

    int word_size = words[0].size();
    int words_step = words.size() * word_size;

    int words_count = words.size();

    int count = 0;
    
    for (int index = 0; index < s.size(); index+= word_size) {
        std::cout << "index: "<< index << std::endl;
        //std::string_view sv_word(s.data() + index, word_size);
        //std::cout << "word:" << sv_word << std::endl;


        for (int inner_index = 0; inner_index < words_count; inner_index++) {
            
            std::string_view sv_word(s.data() + index + inner_index, word_size);

            auto finded_element = frequency_words.find(sv_word);

            if (finded_element != frequency_words.end()) {
                finded_element->second--;
                //frequency_words[sv_word] -+ 1;
                //f//inded_element->first = 0;
                std::cout << "+++" << finded_element->first  << "::" << finded_element->second<< std::endl;
            } else {
                for (auto& [word, frequency] : frequency_words) {
                    frequency = 1;
                }
                continue;
  
            }

            int sum = 0;
            for (auto& [word, frequency] : frequency_words) {
                std::cout << "freq::" << frequency << std::endl;
                sum += frequency;
            }

            if (sum == 0) {
                std::cout << "нашли#######" << std::endl;
            }
        

        }


      //  int sum = 0;
      //  for (auto& [word, frequency] : frequency_words) {
      //      sum += frequency;

      //  }
      //  std::cout << "summa !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
      //  if (sum == 0) {
      //      std::cout << "Нашли наше слово ((((((((((((((" << std::endl;
      //  }

      //  sum = 0;
        //          переходим к следующему окну    
        //foo 1 1 1 1
        //bar 1 0 1 2
        //baz 1 1 1 1
        //      переходим к следующему окну
        if (count % words_count == 0) {
            std::cout << "проверяем каждое окно сумму всех частот слов и если она равна количеству слов" << std::endl;
            std::cout << "значит мы нашли нашу комбинацию" << std::endl;
        }

        count++;

    }

    //std::vector<int> indexes;
    //std::vector<int> res = indexes | std::ranges::iota(10);

    //std::print("{}", res);


    return result;

}

int main() {

    std::string s = "barfoofoobarthefoobarman";

    std::string_view sv(s.data() + 3, 3);
    std::cout << sv << std::endl;

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

    findSubstring(s, words);

    //std::string s
    
    return 0;
}

