#include <iostream>
#include <unordered_map>
#include <string>

int romanToInt(std::string si) {

 
    std::unordered_map<std::string, int> dict {
            {"M_", 1000 },
            {"D_", 500},
            {"CM", -100},
            {"CD", -100},
            {"C_", 100},
            {"L_", 50},
            {"XC", -10},
            {"XL", -10},
            {"X_", 10},
            {"V_", 5},
            {"IX", -1},
            {"IV", -1},
            {"I_", 1}
        };

        int result = 0;
        bool b = si.size() % 2 == 0;
        //si.resize(si.size() + 1);
        if (!b) {
            si += "_";
        }
        std::cout << si << std::endl;
        for (int i = 1; i < si.size(); i += 2) {
           // std::cout << si[i - 1] << si[i];
            std::cout << si[i-1] << si[i] << std::endl;
            std::string characters;
            characters += si[i-1]; //+ si[i];
            characters += si[i];
            std::cout << characters << std::endl;
            result += dict[characters];
        }

//    std::cout << si << std::endl;    
    return result;
}

int main() {
    std::string roman = "XCL";
    std::cout << romanToInt(roman);
    return 0;
}

