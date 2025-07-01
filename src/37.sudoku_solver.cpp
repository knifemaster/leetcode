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
//    std::vector<std::unordered_set<char>> possible_numbers;


    sudoku_state() : 
        horizontal_buckets(9),
        vertical_buckets(9),
        sub_boxes(9)//,
        //possible_numbers(9) 
    { }

 
};


bool is_possible_moves(vector<vector<char>>& board, unordered_set<char>& row_state, unordered_set<char>& column_state, unordered_set<char>& sub_box) {

    return true;
    
}


/*
void backtrack(vector<vector<char>>& board, vector<char>& possible_numbers, int i, int j) {


    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            
            board[i][j]



        }


    }


}
*/



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

// первый проход просчитываем все возможные варианты заполняем buckets и sub_boxes
// back_track () расчитываем возможные наборы чисел и по очереди добавляем в buckets и sub_boxes и помещамем в stack состояние клетки
// просчитываем следующие все возможные варианты(за место этого можно заменять по очереди каждый символ без просчета(с проверкой на валидность))
// как только не получается вставить число возвращаем назад
// refresh sub_boxes and buckets
//

void visit_all_sudoku(vector<vector<char>>& board, sudoku_state& state) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            check_column(board, j, state);
            check_sub_box(board, i, j, state);
        }
        check_row(board, i, state);
    }

            
}

unordered_set<char> exclude_possible_numbers(sudoku_state& state, int i, int j) {
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

void solveSudoku(std::vector<std::vector<char>>& board) {

    print_board(board);

    sudoku_state state;

    visit_all_sudoku(board, state);
    print_all_states(state);

    bool quit = false;

    for (int i = 0; i < 9; i++) {

        for (int j = 0; j < 9; j++) {

            if (board[i][j] == '.') {


                //вставляем число из possible_numbers
                // если следующий шаг нет вставляемых чисел
                // запоминаем i, j в стеке
                // возвращаемся к предидущему состоянию (удаляем вставленное число и рефрешим наши бакеты и боксы )
                // и вставляем другое возможное число
                //
                //if (j == 4) {
                //    return;
                //}
                quit = false;
                auto nums = exclude_possible_numbers(state, i, j);

                if (nums.begin() == nums.end()) {
                    std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
                    std::cout << i << " " << j << std::endl;
                    std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
        
                }

                std::cout << "Possible numbers is" << std::endl;
                for (auto it = nums.begin(); it != nums.end(); ++it) {

                    
                    if (!quit) {
                        board[i][j] = *it;
                        std::cout << *it << " ";   
                        add_number_to_state(state, i, j, *it);
                    }
                    quit = true;
                }
                std::cout << std::endl;

            }

        }

    }    


    return;

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


    solveSudoku(board);


    print_board(board);
/*
    print_board(board);

    sudoku_state state;

    visit_all_sudoku(board, state);
    print_all_states(state);


    for (int i = 0; i < 9; i++) {

        for (int j = 0; j < 9; j++) {

            auto nums = exclude_possible_numbers(state, i, j);

            std::cout << "Possible numbers is" << std::endl;
            for (auto it = nums.begin(); it != nums.end(); ++it) {
                std::cout << *it << " ";    
            }
            std::cout << std::endl;

        }

    }

*/

//    std::cout << check_column(board, 2, state) << std::endl;
//    std::cout << check_row(board, 1, state) << std::endl;
//    std::cout << check_sub_box(board, 1, 1, state) << std::endl;
//    for (const auto& number: state.sub_boxes[0]) {
//        std::cout << number << std::endl;
//    }
    // сначала визит потом чекаем

    //visit_sub_boxes(board, 0, 0, 3, 3);
    //visit_sub_boxes(board, 0, 3, 3, 6);

    return 0;

}
