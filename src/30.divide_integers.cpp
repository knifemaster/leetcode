#include <iostream>
#include <cmath>
#include <limits>

int divide(int dividend, int divisor) {

    bool is_negative;

    if (((dividend > 0) && (divisor > 0)) || ((dividend < 0) && (divisor < 0))) {
        is_negative = false;
    } else {
        is_negative = true;
    }

    if (dividend == std::numeric_limits<int>::max() && divisor == -1) {
        return std::numeric_limits<int>::max(); // Or handle according to specific problem constraints for this edge case
    }
    // Handle INT_MIN / -1 overflow as well
    if (dividend == std::numeric_limits<int>::min() && divisor == -1) {
        return std::numeric_limits<int>::max();
    }


    long long dividend_ll = abs(static_cast<long long>(dividend)); // Use a new variable for the changing dividend
    long long divisor_ll = abs(static_cast<long long>(divisor));

    long long result = 0;

    // A common optimization for the bit shift method:
    // If the divisor is 0, this would be division by zero, which is undefined behavior.
    // Assuming divisor is not 0 based on typical problem constraints.
    if (divisor_ll == 0) {
        // Handle division by zero, e.g., throw an exception or return a specific error value.
        // For this problem, let's assume valid divisor.
        return std::numeric_limits<int>::max(); // Or another appropriate error handling
    }


    for (int exponent = 31; exponent >= 0; exponent--) {
        if ((divisor_ll << exponent) <= dividend_ll) { // Use dividend_ll here
            dividend_ll -= (divisor_ll << exponent);   // Update dividend_ll
            result += (1LL << exponent);
            // The debug prints below are for understanding, remove in final code
            // std::cout << "dividend_ll : " << dividend_ll << std::endl;
            // std::cout << "result : " << result << std::endl;
        }    
    }
    
    result = is_negative ? -result : result;

    // Check for overflow after applying sign
    if (result < std::numeric_limits<int>::min()) {
        return std::numeric_limits<int>::min(); // This case is unlikely if result is built positively and then negated.
    }
    if (result > std::numeric_limits<int>::max()) {
        return std::numeric_limits<int>::max();
    }
    
    return static_cast<int>(result); // Cast to int for final return
}


int main() {
    int dividend = 7;
    int divisor = -3;

    std::cout << divide(dividend, divisor);

    return 0;
}
