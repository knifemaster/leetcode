#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>


bool isSafe(std::vector<std::string>& board, int row, int col, int n) {
        for (int i = 0; i < row; i++) {
            if (board[i][col] == 'Q') {
                return false;
            }
        }
        
        for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
            if (board[i][j] == 'Q') {
                return false;
            }
        }
        
        for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
            if (board[i][j] == 'Q') {
                return false;
            }
        }
        
        return true;
}


void backtrack(std::vector<std::string>& board, int row, std::vector<std::vector<std::string>>& solutions, int n) {
        if (row == n) {
            solutions.push_back(board);
            
            return;
        }
        
        for (int col = 0; col < n; col++) {
            if (isSafe(board, row, col, n)) {
                board[row][col] = 'Q';  // Place queen
                backtrack(board, row + 1, solutions, n);
                board[row][col] = '.';  // Remove queen (backtrack)
            }
        }
}



std::vector<std::vector<std::string>> solveNQueens(int n) {

    std::vector<std::vector<std::string>> solution;
    int column_size = n;
    int row_size = n;

    std::vector<std::string> board(n, std::string(n, '.'));
    backtrack(board, 0, solution, n);
    
    return solution;


}


void printSolutions(std::vector<std::vector<std::string>>& solutions, int n) {
    std::cout << "Total solutions for " << n << " queens: " << solutions.size() << "\n\n";
        
        for (int i = 0; i < solutions.size(); i++) {
            std::cout << "Solution " << i + 1 << ":\n";
            for (const std::string& row : solutions[i]) {
                for (char c : row) {
                    std::cout << c << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
}
    
void printOneSolution(std::vector<std::vector<std::string>>& solutions, int n) {
        if (solutions.empty()) {
            std::cout << "No solution found for " << n << " queens.\n";
            return;
        }
        
        std::cout << "One solution for " << n << " queens:\n";
        for (const std::string& row : solutions[0]) {
            for (char c : row) {
                std::cout << c << " ";
            }
            std::cout << "\n";
        }
}
//}

int main() {
    
    int board_size = 6;
    auto solved = solveNQueens(board_size);
    //printOneSolution(solved, board_size);
    std::cout << solved.size();
    printSolutions(solved, board_size);
}
