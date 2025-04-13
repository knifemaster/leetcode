class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) {
            return "";
        }
        
        string result;
        int index = 0;
        
        while (true) {
            if (index >= strs[0].size()) {
                return result;
            }
            
            char current_char = strs[0][index];
            
            for (const auto& word : strs) {
                if (index >= word.size() || word[index] != current_char) {
                    return result;
                }
            }
            
            result += current_char;
            index++;
        }
    }
};

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>


using namespace std;

string longestCommonPrefix(vector<string>& strs) {
    int index = 0;
    char character;
    string result;
    bool is_difference = true;
    //std::cout << strs[0][0];
    std::string word;
    while (true) {
        word = strs[0];
        character = word[index];
        for (const auto& word : strs) {
            if (word[index] == character) {
                continue;
            } else {
                is_difference = false;
                break;
            }
        }
        if(!is_difference) {
            break;
        }
        result += character;
        index++;
    }


    return result;
}


int main() {
    vector<string> vecs = {"flower", "flow", "flight"};
    vector<string> vec_one_word = {"a"};
    vector<string> vec_flowers = {"flower", "flower", "flower"};
    cout << longestCommonPrefix(vec_flowers)<< "b" << endl; 


}
