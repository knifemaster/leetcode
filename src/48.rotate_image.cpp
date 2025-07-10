#include <iostream>
#include <vector>

void rotate(std::vector<std::vector<int>>& matrix) {

}

void print_matrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void print_indexes(const std::vector<std::vector<int>>& matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = matrix[0].size() - 1; j > -1; j--) {
            std::cout << matrix[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

void rotate_matrix(std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n/2; j++) {
            std::swap(matrix[i][j], matrix[i][n-1-j]);
        }
    }
}

int main() {
    std::vector<std::vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    std::cout << "Original matrix:" << std::endl;
    print_matrix(mat);
    
    std::cout << "\nIndex view:" << std::endl;
    print_indexes(mat);
    
    rotate_matrix(mat);
    
    std::cout << "\nRotated matrix:" << std::endl;
    print_matrix(mat);

    return 0;
}
