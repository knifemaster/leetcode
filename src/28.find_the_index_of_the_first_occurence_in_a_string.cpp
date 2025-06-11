#include <iostream>
#include <string>
#include <string_view>

int strStr(std::string haystack, std::string needle) {


    std::string_view sv = haystack;
    std::string_view substring = needle;

    return sv.find(substring);

    //for (int i = 0; i < haystack.size(); i++) {
    //     if (haystack[i] == needle[j]) {
    //    }
    //}

    //return 0;
}


int main() {
    
    std::string haystack = "sadbutsad";
    std::string needle = "sade";
    std::cout << strStr(haystack, needle) << std::endl;
    return 0;
}
