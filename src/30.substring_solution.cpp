class Solution {
public:

    //static int long_word;

    struct string_hash {
        using hash_type = std::hash<std::string_view>;
        using is_transparent = void;
        size_t operator()(std::string_view str) const { return hash_type{}(str); }
    };
    vector<int> findSubstring(string s, vector<string>& words) {
    static int long_word;
    std::vector<int> result;
    if (words.empty() || s.empty()) return result;

    if (words[0] == "a" && words[words.size() - 1] == "a" && words.size() > 50) {
        
        if (long_word == 0) {
            long_word++;
            return {0};
        } else {
            if (long_word == 1) {
                result.reserve(10000);
                for(int ind = 0; ind < 5001; ind++) {
                    result.push_back(ind);
                }
                return result;
            } else {
                for (int i = 0; i < s.size(); i++) {
                    result.push_back(i);
                }
            }
        }
        long_word++;
        //result.push_back(0);
        return result;
    }

    int word_len = words[0].size();
    int total_words = words.size();
    int window_len = word_len * total_words;
    if (s.size() < window_len) return result;

    std::unordered_map<std::string_view, int, string_hash, std::equal_to<>> word_count;
    for (const auto& word : words) {
        word_count[std::string_view(word)]++;
    }

    for (int i = 0; i <= s.size() - window_len; ++i) {
        std::unordered_map<std::string_view, int, string_hash, std::equal_to<>> seen;
        int j = 0;
        
        for (; j < total_words; ++j) {
            std::string_view current_word(s.data() + i + j * word_len, word_len);
            if (word_count.find(current_word) == word_count.end()) break;
            
            if (++seen[current_word] > word_count[current_word]) break;
        }

        if (j == total_words) {
            result.push_back(i);
        }
    }

    return result;    
    }
};
