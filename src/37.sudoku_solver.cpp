#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <utility>
#include <list>

using namespace std;


struct sudoku_state {
    std::vector<std::unordered_set<char>> horizontal_buckets;
    std::vector<std::unordered_set<char>> vertical_buckets;
    std::vector<std::unordered_set<char>> sub_boxes; 
    std::vector<std::pair<int, int>> empty_cells;


    sudoku_state() : 
        horizontal_buckets(9),
        vertical_buckets(9),
        sub_boxes(9)//,
    { }

 
};





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
void visit_sub_boxes(vector<vector<char>>& board, int i_begin, int j_begin, int i_end, int j_end, sudoku_state& state) {


    int sub_box_index = (i_begin / 3) * 3 + (j_begin / 3);
  

    for (int i = i_begin; i < i_end; i++) {
        for (int j = j_begin; j < j_end; j++) {

            if (board[i][j] == '.') {
                continue;
            }

            auto finded_element = state.sub_boxes[sub_box_index].find(board[i][j]);
            
            if (finded_element != state.sub_boxes[sub_box_index].end()) {
                //return false; нашли элемент то исключаем его из списка
                continue;
            } else {
                state.sub_boxes[sub_box_index].insert(board[i][j]);
            }

            //std::cout << i << j << " ";
            //std::cout << board[i][j] <<" ";
            //int sub_box_index = (i / 3) * 3 + (j / 3);
            //std::cout << sub_box_index << std::endl;

        }
        std::cout << std::endl;
    }

    //int sub_box_index = (i / 3) * 3 + (j / 3);

    return;

}

bool check_sub_box(vector<vector<char>>& board, int i, int j, sudoku_state& state) {

    int sub_box_index = (i / 3) * 3 + (j / 3);

    if (board[i][j] == '.') {
        return false;    
    }

    auto finded_element = state.sub_boxes[sub_box_index].find(board[i][j]);

    if (finded_element != state.sub_boxes[sub_box_index].end()) {
        return false;
    }
    
    state.sub_boxes[sub_box_index].insert(board[i][j]);

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
 
//1 9 8 3 4 2 5 6 7 
//8 5 9 7 6 1 4 2 3 
//4 2 6 8 5 3 7 9 1 
//7 1 3 9 2 4 8 5 6 
//9 6 1 5 3 7 2 8 4 
//2 8 7 4 1 9 6 3 5 

// первый проход просчитываем все возможные варианты заполняем buckets и sub_boxes
// back_track () расчитываем возможные наборы чисел и по очереди добавляем в buckets и sub_boxes и помещамем в stack состояние клетки
// просчитываем следующие все возможные варианты(за место этого можно заменять по очереди каждый символ без просчета(с проверкой на валидность))
// как только не получается вставить число возвращаем назад
// refresh sub_boxes and buckets
//

unordered_set<char> include_possible_numbers(sudoku_state& state, int i, int j) {
    unordered_set<char> possible_numbers { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    
//  for (int i = 0; i < 9; i++) {

    std::cout << "excluding numbers " << std::endl;

    for (auto it = state.horizontal_buckets[i].begin(); it != state.horizontal_buckets[i].end(); ++it) {
        std::cout << *it << " ";
        possible_numbers.erase(*it);
    }

    for (auto it = state.vertical_buckets[j].begin(); it != state.vertical_buckets[j].end(); ++it) {
        possible_numbers.erase(*it);
    }

    int sub_box_index = (i / 3) * 3 + (j / 3);
    
    for (auto it = state.sub_boxes[sub_box_index].begin(); it != state.sub_boxes[sub_box_index].end(); ++it) {
        possible_numbers.erase(*it);
    }


    return possible_numbers;


}

void visit_all_sudoku(vector<vector<char>>& board, sudoku_state& state) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            check_column(board, j, state);
            check_sub_box(board, i, j, state);
        }
        check_row(board, i, state);
    }

    vector<tuple<int, int, int>> cells_with_options;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') {
                auto possible = include_possible_numbers(state, i, j);
                cells_with_options.emplace_back(possible.size(), i, j);
            }
        }
    }

    sort(cells_with_options.begin(), cells_with_options.end());
    for (auto& [cnt, i, j] : cells_with_options) {
        state.empty_cells.emplace_back(i, j);
    }
            
}



void add_number_to_state(sudoku_state& state, int i, int j, char ch) {
    int sub_box_index = (i / 3) * 3 + (j / 3);
    state.horizontal_buckets[i].insert(ch);
    state.vertical_buckets[j].insert(ch);
    state.sub_boxes[sub_box_index].insert(ch);

}


void print_all_states(sudoku_state& state) {
    std::cout << "rows____________" << std::endl;


    for (int i = 0; i < 9; i++) {

        std::cout << "index " << i << std::endl;

        for (auto it = state.horizontal_buckets[i].begin(); it != state.horizontal_buckets[i].end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "columns_________" << std::endl;


    for (int i = 0; i < 9; i++) {

        std::cout << "index " << i << std::endl;

        for (auto it = state.vertical_buckets[i].begin(); it != state.vertical_buckets[i].end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    std::cout << "sub_boxes_______" << std::endl;
    

    for (int i = 0; i < 9; i++) {

        std::cout << "index " << i << std::endl;

        for (auto it = state.sub_boxes[i].begin(); it != state.sub_boxes[i].end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


}



bool solve_sudoku(std::vector<std::vector<char>>& board, sudoku_state& state, int index) {
    if (index == state.empty_cells.size()) {
        return true;
    }

    auto [i, j] = state.empty_cells[index];

    auto possible = include_possible_numbers(state, i, j);

    int sub_box_index = (i / 3) * 3 + j / 3;

    for (char num : possible) {
        board[i][j] = num;
        state.horizontal_buckets[i].insert(num);
        state.vertical_buckets[j].insert(num);
        state.sub_boxes[sub_box_index].insert(num);

        if (solve_sudoku(board, state, index+1)) {
            return true;
        }

        board[i][j] = '.';
        state.horizontal_buckets[i].erase(num);
        state.vertical_buckets[j].erase(num);
        state.sub_boxes[sub_box_index].erase(num);
    }

    return false;



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


    sudoku_state state;

    visit_all_sudoku(board, state);

     if (solve_sudoku(board, state, 0)) {
        cout << "Solved successfully!" << endl;
        print_board(board);
    }


    return 0;

}
