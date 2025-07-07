#include <iostream>
#include <print>
#include <vector>
#include <algorithm>
#include <stack>



int trap(std::vector<int>& height) {

    std::stack<int> temp;
    int volume = 0;

    for (int index = 0; index < height.size(); ++index) {
        
        while(!temp.empty() && height[index] > height[temp.top()]) {
            int top = temp.top();
            temp.pop();

            if (temp.empty()) {
                break;
            }

            int distance = index - temp.top() - 1;
            int aligned_height = std::min(height[index], height[temp.top()]) - height[top];        
            std::cout << "distance: " << distance << std::endl;
            std::cout << "boundedHeight: " << aligned_height << std::endl;

            volume += distance * aligned_height;
            std::cout << "volume: " << volume << std::endl;

        }        

        temp.push(index);

    }

    return volume;

}


int main() {

    std::vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    std::cout << "trapped water: " << trap(height) << std::endl;

    return 0;

}
