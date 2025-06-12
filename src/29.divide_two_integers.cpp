#include <iostream>
#include <cmath>
#include <limits>

int divide(int dividend, int divisor) {
    
    if (dividend == 2147483647 && divisor == -1) {
        return -dividend;
    }

    bool is_negative;

    if (((dividend > 0) && (divisor > 0)) || ((dividend < 0) && (divisor < 0))) {
        is_negative = false;
    } else {
        is_negative = true;
    }

    if (dividend == std::numeric_limits<int>::max() && divisor == -1) {
        return std::numeric_limits<int>::max();
    }

    long long dividend_abs = std::abs(static_cast<long long>(dividend));
    long long divisor_abs = std::abs(static_cast<long long>(divisor));

    long long result = 0;

    
    for (int exponent = 31; exponent >= 0; exponent--) {
        if ((divisor_abs << exponent) <= dividend_abs) {
            dividend_abs -= (divisor_abs << exponent);
            result += (1LL << exponent);
        //    std::cout << "dividend : " << dividend << std::endl;
        //    std::cout << "result : " << result << std::endl;
        }       

    }
    

    result = is_negative ? -result : result;


    if (result < std::numeric_limits<int>::min()) {
        return std::numeric_limits<int>::min();
    }

    if (result > std::numeric_limits<int>::max()) {
        return std::numeric_limits<int>::max();
    }
    

    return static_cast<int>(result);

}


int main() {
    int dividend = 7;
    int divisor = -3;

    std::cout << divide(dividend, divisor);


    return 0;
}
