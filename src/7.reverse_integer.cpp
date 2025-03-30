#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

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
