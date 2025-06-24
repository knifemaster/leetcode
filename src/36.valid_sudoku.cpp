#include <iostream>
#include <vector>
#include <unordered_set>



class Solution {
public:

bool visit_rows(std::vector<std::vector<char>>& board) {

    std::vector<std::unordered_set<char>> buckets(9);

    for(int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            char current_char = board[i][j];
            if (current_char == '.') {
                continue;
            }

            if (current_char >= '0' && current_char <= '9') {

                auto finded_element = buckets[i].find(current_char);

                if (finded_element != buckets[i].end()) {
                    return false;
                } else {
                    buckets[i].insert(current_char);
                }

            }

        }
    }

    return true;
}


bool visit_columns(std::vector<std::vector<char>>& board) {
  
    std::vector<std::unordered_set<char>> buckets(9);
    
    for(int j = 0; j < board[0].size(); j++) {
        for (int i = 0; i < board.size(); i++) {
            char current_char = board[i][j];
            if (current_char == '.') {
                continue;
            }

            if (current_char >= '0' && current_char <= '9') {
                
                auto finded_element = buckets[j].find(current_char);

                if (finded_element != buckets[j].end()) {
                    return false;
                } else {
                    buckets[j].insert(current_char);
                }

            }

        }
    }

    return true;
}



bool check_sub_boxes(std::vector<std::vector<char>>& board) {
    std::vector<std::unordered_set<char>> sub_boxes(9);
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[0].size(); ++j) {
            char current_char = board[i][j];
            if (current_char == '.') {
                continue;
            }

            int sub_box_index = (i / 3) * 3 + (j / 3);

            if (!sub_boxes[sub_box_index].insert(current_char).second) {
                return false;
            }
        }
    }

    return true;
}


bool isValidSudoku(std::vector<std::vector<char>>& board) {
    if (visit_rows(board) && visit_columns(board) && check_sub_boxes(board)) {
        return true;
    } else {
        return false;
    }

}


};



/*
bool visit_rows(std::vector<std::vector<char>>& board) {

    std::vector<std::unordered_set<char>> buckets(9);

    for(int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            
            if (board[i][j] == '.') {
                continue;
            }

            if (board[i][j] >= '0' && board[i][j] <= '9') {

                auto finded_element = buckets[i].find(board[i][j]);

                if (finded_element != buckets[i].end()) {
                    return false;
                } else {
                    buckets[i].insert(board[i][j]);
                }

            }

            //std::cout << i << j << "  ";
        }
        //std::cout << std::endl;
    }

    return true;
}


bool visit_columns(std::vector<std::vector<char>>& board) {
  
    std::vector<std::unordered_set<char>> buckets(9);
    
    for(int j = 0; j < board[0].size(); j++) {
        for (int i = 0; i < board.size(); i++) {
        
            if (board[i][j] == '.') {
                continue;
            }

            if (board[i][j] >= '0' && board[i][j] <= '9') {
                
                auto finded_element = buckets[j].find(board[i][j]);

                if (finded_element != buckets[j].end()) {
                    return false;
                } else {
                    buckets[j].insert(board[i][j]);
                }

            }

        //    std::cout << i << j << " ";
        }
        //std::cout << std::endl;
    }

    return true;
}



bool check_sub_boxes(std::vector<std::vector<char>>& board) {
    std::vector<std::unordered_set<char>> sub_boxes(9);
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[0].size(); ++j) {
            char current_char = board[i][j];
            if (current_char == '.') {
                continue;
            }

            int sub_box_index = (i / 3) * 3 + (j / 3);

            if (!sub_boxes[sub_box_index].insert(current_char).second) {
                return false;
            }
        }
    }

    return true;
}


bool isValidSudoku(std::vector<std::vector<char>>& board) {
    if (visit_rows(board) && visit_columns(board) && check_sub_boxes(board)) {
        return true
    } else {
        return false;
    }


}





void print_board(std::vector<std::vector<char>>& board) {
    for (const auto& row : board) {
        for (const auto& element: row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

}


int main() {

    std::vector<std::vector<char>> board = {
                                            {'5','3','.','.','7','.','.','.','.'}
                                            ,{'6','.','.','1','9','5','.','.','.'}
                                            ,{'.','9','8','.','.','.','.','6','.'}
                                            ,{'8','.','.','.','6','.','.','.','3'}
                                            ,{'4','.','.','8','.','3','.','.','1'}
                                            ,{'7','.','.','.','2','.','.','.','6'}
                                            ,{'.','6','.','.','.','.','2','8','.'}
                                            ,{'.','.','.','4','1','9','.','.','5'}
                                            ,{'.','.','.','.','8','.','.','7','9'}
                                            };

    print_board(board);

    std::cout <<  visit_rows(board) << std::endl;
    std::cout <<  visit_columns(board) << std::endl;
    std::cout << check_sub_boxes(board) << std::endl;

    return 0;

}

*/
