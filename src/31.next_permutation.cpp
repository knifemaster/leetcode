
/*
#include <ranges>
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
    int i = nums.size() - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
    }

    // Если такой элемент 'i' найден (то есть, массив не отсортирован убывающе)
    if (i >= 0) {
        // 2. Найти самый правый элемент 'j', который больше, чем nums[i].
        // Этот 'j' будет наименьшим элементом справа от 'i', который больше nums[i].
        int j = nums.size() - 1;
        while (nums[j] <= nums[i]) {
            j--;
        }
        // 3. Поменять местами nums[i] и nums[j].
        std::swap(nums[i], nums[j]);
    }

    // 4. Развернуть (отсортировать по возрастанию) подмассив справа от 'i'.
    // Этот шаг гарантирует, что полученная перестановка будет следующей лексикографически большей.
    // Если 'i' не был найден (массив изначально был отсортирован убывающе),
    // то i равен -1, и мы разворачиваем весь массив, получая наименьшую перестановку.
    std::reverse(nums.begin() + i + 1, nums.end());  
    }
};
*/

#include <ranges>
#include <algorithm>
#include <vector>
#include <iostream>
#include <print>

void nextPermutation(std::vector<int>& nums) {
    int i = nums.size() - 2;

    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
        std::cout << i << std::endl;
    }

    if (i >= 0) {
        int j = nums.size() - 1;
        while (nums[j] <= nums[i]) {
            j--;
        }
        std::swap(nums[i], nums[j]);
    }

    std::reverse(nums.begin() + i + 1, nums.end());

}

int main() {

    std::vector<int> numbers {3, 2, 1};
    std::print("{}", numbers);
    nextPermutation(numbers);
    std::print("{}", numbers);

}



