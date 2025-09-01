class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> anagramMap;
        for (string s : strs) {
            string key = s;
            sort(key.begin(), key.end());
            anagramMap[key].push_back(s);
        }
        vector<vector<string>> result;
        for (auto it = anagramMap.begin(); it != anagramMap.end(); it++) {
            result.push_back(it->second);
        }
        return result;    
    }
};
