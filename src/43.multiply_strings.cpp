#include <iostream>
#include <string>
#include <vector>


std::string multiply(std::string& num1, std::string& num2) {
    if (num1 == "0" || num2 == "0") {
        return "0";
    }
    
    int n1_size = num1.size();
    int n2_size = num2.size();

    std::vector<int> res_multiply(n1_size + n2_size, 0);
    

    for (int i = n1_size - 1; i >= 0; --i) {
        for (int j = n2_size - 1; j >= 0; --j) {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + res_multiply[i + j + 1]; 
            res_multiply[i + j + 1] = sum % 10; 
            res_multiply[i + j] += sum / 10;    
        }
    }
    
 
    std::string result;
    for (int num : res_multiply) {
        if (!(result.empty() && num == 0)) { 
            result.push_back(num + '0');
        }
    }
    
    if (result.empty()) {
        return "0";
    } else {
        return result;
    }

    
}


int main() {
    std::string number1 = "12";
    std::string number2 = "5";

    std::cout << multiply(number1, number2) << std::endl;




    return 0;
}
