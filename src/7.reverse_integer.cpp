#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

#include <climits>


class Solution {
public:
    int reverse(int x) {
        int rev = 0;

        while (x != 0) {
            int pop = x % 10;
            x /= 10;

            // Проверка на переполнение перед умножением и сложением
            if (rev > INT_MAX/10 || (rev == INT_MAX/10 && pop > 7)) return 0;
            if (rev < INT_MIN/10 || (rev == INT_MIN/10 && pop < -8)) return 0;

            rev = rev * 10 + pop;
        }

        return rev;
    }
};


int reverse(int x) {
    int number;
    bool is_negative = false;
    if (x < 0) {
        number = std::abs(x);
        is_negative = true;
    }

    int remainder;
    int power = 0;
    int result = 0;

    while(number > 0) {        
        remainder = number % 10;
        number /= 10;
        result += remainder * std::pow(10, power);
        power++; 
    }

    if (is_negative) {
        result = -result;
    }
    return result;

}

int main() {
    
    int x = 321;    
    int res = reverse(x);    
    std::cout << "rev " << res << std::endl;
    

    return 0;
}
