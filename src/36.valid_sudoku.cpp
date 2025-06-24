#include <iostream>
#include <vector>
#include <unordered_set>


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



/*
bool check_sub_boxes(std::vector<std::vector<char>>& board) {
    
    std::vector<std::unordered_set<char>> sub_boxes(9);

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {

            if (i >= 0 && i <= 2 && j >= 0 && j <= 2) {
                auto finded = sub_boxes[0].find(board[i][j]);

                if (finded != sub_boxes[0].end()) {
                    return false;
                } else {
                    sub_boxes[0].insert(board[i][j]);
                }
            }

            if (i >= 0 && i <= 2 && j >= 3 && j <= 5) {
                auto finded = sub_boxes[1].find(board[i][j]);

                if (finded != sub_boxes[1].end()) {
                    return false;
                } else {
                    sub_boxes[1].insert(board[i][j]);
                }
            }

            if (i >= 0 && i <= 2 && j >= 6 && j <= 8) {
                auto finded = sub_boxes[2].find(board[i][j]);

                if (finded != sub_boxes[2].end()) {
                    return false;
                } else {
                    sub_boxes[2].insert(board[i][j]);
                }


            }

            if (i >= 3 && i <= 5 && j >= 0 && j <= 2) {
                auto finded = sub_boxes[3].find(board[i][j]);

                if (finded != sub_boxes[3].end()) {
                    return false;
                } else {
                    sub_boxes[3].insert(board[i][j]);
                }

            }

            if (i >= 3 && i <= 5 && j >= 3 && j <= 5) {
                auto finded = sub_boxes[4].find(board[i][j]);

                if (finded != sub_boxes[4].end()) {
                    return false;
                } else {
                    sub_boxes[4].insert(board[i][j]);
                }

            }

            if (i >= 3 && i <= 5 && j >= 6 && j <= 8) {
                auto finded = sub_boxes[5].find(board[i][j]);

                if (finded != sub_boxes[5].end()) {
                    return false;
                } else {
                    sub_boxes[5].insert(board[i][j]);
                }
            }

            if (i >= 6 && i <= 8 && j >= 0 && j <= 2) {
                auto finded = sub_boxes[6].find(board[i][j]);

                if (finded != sub_boxes[6].end()) {
                    return false;
                } else {
                    sub_boxes[6].insert(board[i][j]);
                }

            }

            if (i >= 6 && i <= 8 && j >= 3 && j <= 5) {
                auto finded = sub_boxes[7].find(board[i][j]);

                if (finded != sub_boxes[7].end()) {
                    return false;
                } else {
                    sub_boxes[7].insert(board[i][j]);
                }

            }

            if (i >= 6 && i <= 8 && j >= 6 && j <= 8) {
                auto finded = sub_boxes[8].find(board[i][j]);

                if (finded != sub_boxes[8].end()) {
                    return false;
                } else {
                    sub_boxes[8].insert(board[i][j]);
                }

            }

        }
    }



    return true;
}
*/

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
