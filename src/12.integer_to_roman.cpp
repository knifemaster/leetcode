    static const string thousands[] = {"", "M", "MM", "MMM"};
    static const string hundreds[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    static const string tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    static const string ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};

class Solution {

public:

    string intToRoman(int num) {
        
        return thousands[num / 1000] + hundreds[(num % 1000) / 100] + tens[(num % 100) / 10] + ones[num % 10];

    }
};





#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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

vector<string> ones = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
vector<string> tens = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
vector<string> hundreds = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
vector<string> thousands = {"", "M", "MM", "MMM", "MMMM"};

string intToRoman(int num) {



    int thous = num / 1000;
    int hundr = (num / 100) % 10;
    int ten = (num/10) % 10;
    int one = num % 10;

    std::cout << num << std::endl;
    std::cout << thous << std::endl;
    std::cout << hundr << std::endl;
    std::cout << ten << std::endl;
    std::cout << one << std::endl;
    
    return thousands[thous] + hundreds[hundr] + tens[ten] + ones[one];
    
    //int thousands = num / 1000;
    //int
    

    return "";
}

int main() {

    int number = 58;
    std::cout << intToRoman(number);

}
