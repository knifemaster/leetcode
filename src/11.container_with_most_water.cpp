#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

int maxArea(vector<int>& height) {
    for (const auto& h : height) {
        std::cout << h << "  ";
    }
    std::cout << std::endl;

    int left = 0;
    int right = height.size() - 1;

    int max_area = 0;

    while (left != right) {
        int difference = std::abs(height[left] - height[right]);
        int min_height = std::min(height[left], height[right]);
        int square = min_height * min_height;
        int width = right - left;

        std::cout << square << " " << width << " " << min_height * width << std::endl;
        int area = min_height* width;
        max_area = std::max(area, max_area);

        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }

    }

    return max_area;
}


int main() {

    vector<int> height = { 1, 8, 6, 2, 5, 4, 8, 3, 7};

    std::cout << std::endl;
    std::cout << maxArea(height);


    return 0;
}
