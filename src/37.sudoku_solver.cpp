#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stack>


using namespace std;


struct sudoku_state {
    std::vector<std::unordered_set<char>> horizontal_buckets;
    std::vector<std::unordered_set<char>> vertical_buckets;
    std::vector<std::unordered_set<char>> sub_boxes; 
    std::stack<std::pair<int, int>> stack_of_indexes; // possibly not needed
    // для возврата предидущих состояний
    //
    std::unordered_set<char> possible_numbers;

    // Constructor to initialize the vectors with 9 elements
    sudoku_state() : 
        horizontal_buckets(9),
        vertical_buckets(9),
        sub_boxes(9)
    {
        possible_numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    }

    void refresh_possible_numbers() {
        possible_numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9}; 
    }

    void exclude_possible_numbers(int i, int j) {
        // здесь чекаем и исключаем возможные комбинации горизонталь вертикаль и sub_box
        // 

    }
};


bool is_possible_moves(vector<vector<char>>& board, unordered_set<char>& row_state, unordered_set<char>& column_state, unordered_set<char>& sub_box) {


    return true;
    
}


void backtrack(vector<vector<char>>& board, vector<char>& possible_numbers, int i, int j) {

/*
    for () {
        for () {

        }
    }

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {

            backtrack(board, possible_numbers, i, j);

        }

    }

    //backtrack(board, possible_numbers);
*/
}


void solveSudoku(std::vector<std::vector<char>>& board) {

    sudoku_state state;
    


    return;

}

void print_board(vector<vector<char>>& board) {
    
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            std::cout << board[i][j] << " ";

        }
        std::cout << std::endl;
    }

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            std::cout << i << j << "  ";
        }
        std::cout << std::endl;
    }


    return;
}


// первый проход по всем sub_box
// следующие будем исключать и включать новые числа
void visit_sub_boxes(vector<vector<char>>& board, int i_begin, int j_begin, int i_end, int j_end) {

    for (int i = i_begin; i < i_end; i++) {
        for (int j = j_begin; j < j_end; j++) {
  
            //std::cout << i << j << " ";
            //std::cout << board[i][j] <<" ";

            int sub_box_index = (i / 3) * 3 + (j / 3);

            std::cout << sub_box_index << std::endl;

        }
        std::cout << std::endl;
    }

    //int sub_box_index = (i / 3) * 3 + (j / 3);

    return;

}

bool check_sub_box(vector<vector<char>>& board, int i, int j, sudoku_state& state) {

    int sub_box_index = (i / 3) * 3 + (j / 3);

    auto finded_element = state.sub_boxes[sub_box_index].find(board[i][j]);

    if (finded_element != state.sub_boxes[sub_box_index].end()) {
        return false;
    }

    return true;
}

bool check_row(vector<vector<char>>& board, int i, sudoku_state& state) {

    for (int j = 0; j < board[0].size(); j++) {
        if (board[i][j] == '.') {
            continue;
        }

        auto finded_element = state.horizontal_buckets[i].find(board[i][j]);
        
        if (finded_element != state.horizontal_buckets[i].end()) {
            return false;
        }

        state.horizontal_buckets[i].insert(board[i][j]);

    }
    
    return true;

}

bool check_column(vector<vector<char>>& board, int j, sudoku_state& state) {

    for (int i = 0; i < board.size(); i++) {
        
        if (board[i][j] == '.') {
            continue;
        }

        auto finded_element = state.vertical_buckets[j].find(board[i][j]);

        if (finded_element != state.vertical_buckets[j].end()) {
            return false;
        }

        state.vertical_buckets[j].insert(board[i][j]);

    }

    return true;
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


    print_board(board);

    sudoku_state state;

    std::cout << check_column(board, 2, state) << std::endl;
    std::cout << check_row(board, 1, state) << std::endl;
    
    std::cout << check_sub_box(board, 1, 1, state) << std::endl;
   
    // сначала визит потом чекаем

    //visit_sub_boxes(board, 0, 0, 3, 3);
    //visit_sub_boxes(board, 0, 3, 3, 6);

    return 0;

}
