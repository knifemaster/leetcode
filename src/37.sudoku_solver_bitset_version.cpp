#include <iostream>
#include <vector>
#include <bitset>
#include <array>

using namespace std;

struct SudokuSolver {
    array<bitset<9>, 9> rows;
    array<bitset<9>, 9> cols;
    array<bitset<9>, 9> boxes;
    vector<pair<uint8_t, uint8_t>> empty_cells;
    
    void visit_all_sudoku(vector<vector<char>>& board) {
        for (uint8_t i = 0; i < board.size(); ++i) {
            for (uint8_t j = 0; j < board.size(); ++j) {
                if (board[i][j] != '.') {
                    uint8_t num = board[i][j] - '1';
                    rows[i].set(num);
                    cols[j].set(num);
                    boxes[(i/3)*3 + j/3].set(num);
                }
            }
        }
        
        vector<tuple<uint8_t, uint8_t, uint8_t>> cells;
        for (uint8_t i = 0; i < board.size(); ++i) {
            for (uint8_t j = 0; j < board.size(); ++j) {
                if (board[i][j] == '.') {
                    uint8_t count = (rows[i] | cols[j] | boxes[(i/3)*3 + j/3]).count();
                    cells.emplace_back(9 - count, i, j);
                }
            }
        }
        
        // Сортируем по количеству возможных вариантов (наименьшие сначала)
        sort(cells.begin(), cells.end());
        for (auto& [cnt, i, j] : cells) {
            empty_cells.emplace_back(i, j);
        }
    }
    
    bool solve(vector<vector<char>>& board, size_t index) {
        if (index == empty_cells.size()) {
            return true;
        }
        
        auto [i, j] = empty_cells[index];
        bitset<9> used = rows[i] | cols[j] | boxes[(i/3)*3 + j/3];
        
        // Быстрая проверка если только один вариант
        // подходящего числа
        if (used.count() == 8) {
            uint8_t num = 0;
            while (used.test(num)) {
                num++;
            }

            return place_number(board, i, j, num, index);
        }
        
        // Перебираем все возможные числа
        for (uint8_t num = 0; num < 9; ++num) {
            if (!used.test(num)) {
                if (place_number(board, i, j, num, index)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool place_number(vector<vector<char>>& board, uint8_t i, uint8_t j, uint8_t num, size_t index) {
        board[i][j] = '1' + num;
        rows[i].set(num);
        cols[j].set(num);
        boxes[(i/3)*3 + j/3].set(num);
        
        // Рекурсивно решаем дальше
        if (solve(board, index + 1)) {
            return true;
        }
        
        // Возвращаем к предидущему состоянию изменения если не решили
        board[i][j] = '.';
        rows[i].reset(num);
        cols[j].reset(num);
        boxes[(i/3)*3 + j/3].reset(num);
        
        return false;
    }
};

void solve_sudoku(vector<vector<char>>& board) {
    SudokuSolver solver;
    solver.visit_all_sudoku(board);
    solver.solve(board, 0);
}

void print_sudoku(vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    vector<vector<char>> board = {{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
                                  {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
                                  {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
                                  {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
                                  {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
                                  {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
                                  {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
                                  {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
                                  {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};

    print_sudoku(board);
    
    solve_sudoku(board);
    
    print_sudoku(board);
    
}
