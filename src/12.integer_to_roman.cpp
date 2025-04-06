#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

std::unordered_map<int, string> dictionary = {
    { 1, "I" },
    { 5, "V" },
    { 10, "X" },
    { 50, "L" },
    { 100, "C" },
    { 500, "D" },
    { 1000, "M" }
};

string intToRoman(int num) {
    
    int thousands = num / 1000;
    int hundreds = (num / 100) % 10;
    int tens = (num/10) % 10;
    int ones = num % 10;

    std::cout << num << std::endl;
    std::cout << thousands << std::endl;
    std::cout << hundreds << std::endl;
    std::cout << tens << std::endl;
    std::cout << ones << std::endl;
    
    
    //int thousands = num / 1000;
    //int
    

    return "";
}

int main() {

    int number = 3749;
    intToRoman(number);

}
