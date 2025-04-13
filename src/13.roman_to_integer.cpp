        static constexpr const int values[256] = {
            ['I'] = 1,
            ['V'] = 5,
            ['X'] = 10,
            ['L'] = 50,
            ['C'] = 100,
            ['D'] = 500,
            ['M'] = 1000
        };

class Solution {
public:
    int romanToInt(string s) {

        
        int result = 0;
        const int n = s.size();
        
        // 2. Убираем проверки длины через добавление нулевого символа
        for (int i = 0; i < n; ++i) {
            int current = values[static_cast<unsigned char>(s[i])];
            
            // 3. Проверяем следующий символ только если он существует
            if (i + 1 < n) {
                int next = values[static_cast<unsigned char>(s[i+1])];
                
                // 4. Оптимизация проверки вычитаемых комбинаций
                if (current < next) {
                    result += next - current;
                    ++i; // Пропускаем следующий символ
                    continue;
                }
            }
            
            result += current;
        }
        
        return result;
  
    }
};


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

