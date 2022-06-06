#include <cassert>
#include <iostream>
#include <map>
#include <vector>

constexpr int lowVal = 1;
constexpr int highVal = 9;
constexpr int OpenSpace = -1;
constexpr int DimSz = 9;
constexpr int BoxSz = 3;

/* Make it easier to index boxes
 * Boxes mapped like:
 * [
 *  [0],[1],[2],
 *  [3],[4],[5],
 *  [6],[7],[8],
 * ]
 */
class Coord{
public:
    int rowNum;
    int colNum;
    Coord()= default;
    Coord(int row, int col): rowNum(row), colNum(col){};
};

std::map<int, Coord> BoxMap;

void makeBoxMap(){
    int count = 0;
    for(int i = 0; i < DimSz; i += BoxSz){
        for(int j = 0; j < DimSz; j += BoxSz){
            BoxMap[count++] = Coord(i, j);
        }
    }
}


/* Helper class to see if a number is used more than once */
class NumsUsed{
public:
    NumsUsed();
    std::map<int, bool> Used;
    bool numInsert(int num);
};

NumsUsed::NumsUsed() {
    for(int i = lowVal; i <= highVal; i++)
        Used[i] = false;
}

bool NumsUsed::numInsert(int num) {
    // Open space cant invalidate puzzle
    if(num == OpenSpace)
        return true;

    // It has been used
    if(Used[num])
        return false;

    // Hasn't been used, add it and say true
    Used[num] = true;
    return true;
}

// Assumption -- Board is a vector of vector of strings
typedef std::vector<std::vector<std::string>> RawSudokuBoard;

class SudokuBoard{
    std::vector<std::vector<int>> board;
    bool rowValid(size_t rowNum);
    bool colValid(size_t colNum);
    bool boxValid(size_t boxNum);
public:
    SudokuBoard(const RawSudokuBoard& rawBoard);
    /*
     * Determine if a 9x9 Sudoku board is valid. Only the filled cells need to be validated
     * according to the following rules:
     *      1. Each row must contain the digits 1-9 without repetition.
     *      2. Each column must contain the digits 1-9 without repetition.
     *      3. Each of the nine 3 x 3 sub-boxes of the grid must contain the digits 1-9 without repetition.
     */
    bool isValidSudoku();
};

// Assumes the input is valid, turn board to numbers
SudokuBoard::SudokuBoard(const RawSudokuBoard& rawBoard) {
    for(const std::vector<std::string>& row : rawBoard){
        std::vector<int> tmp = {};
        for(const std::string & numAsStr : row){
            try{
                tmp.push_back(std::stoi(numAsStr));
            }catch (std::invalid_argument const& ex){
                tmp.push_back(OpenSpace);
            }
        }
        board.push_back(tmp);
    }
}

bool SudokuBoard::rowValid(size_t rowNum){
    NumsUsed nu;
    for(int i = 0; i < DimSz; i++){
        if(!nu.numInsert(board[rowNum][i]))
            return false;
    }
    return true;
}
bool SudokuBoard::colValid(size_t colNum){
    NumsUsed nu;
    for(int i = 0; i < DimSz; i++){
        if(!nu.numInsert(board[i][colNum]))
            return false;
    }
    return true;
}

bool SudokuBoard::boxValid(size_t boxNum){
    NumsUsed nu;
    Coord coord = BoxMap[boxNum];
    for(int i = coord.rowNum; i<coord.rowNum+BoxSz; i++){
        for(int j = coord.colNum; j < coord.colNum+BoxSz; j++){
            if(!nu.numInsert(board[i][j]))
                return false;
        }
    }
    return true;
}


bool SudokuBoard::isValidSudoku() {
    for(int i = 0; i < DimSz; i++){
        if(!rowValid(i) || !colValid(i) || !boxValid(i))
            return false;
    }
    return true;
}

const RawSudokuBoard  rb1 =  {
    {"5","3",".",".","7",".",".",".","."},
    {"6",".",".","1","9","5",".",".","."},
    {".","9","8",".",".",".",".","6","."},
    {"8",".",".",".","6",".",".",".","3"},
    {"4",".",".","8",".","3",".",".","1"},
    {"7",".",".",".","2",".",".",".","6"},
    {".","6",".",".",".",".","2","8","."},
    {".",".",".","4","1","9",".",".","5"},
    {".",".",".",".","8",".",".","7","9"},
};

const RawSudokuBoard rb2 = {
    {"8","3",".",".","7",".",".",".","."},
    {"6",".",".","1","9","5",".",".","."},
    {".","9","8",".",".",".",".","6","."},
    {"8",".",".",".","6",".",".",".","3"},
    {"4",".",".","8",".","3",".",".","1"},
    {"7",".",".",".","2",".",".",".","6"},
    {".","6",".",".",".",".","2","8","."},
    {".",".",".","4","1","9",".",".","5"},
    {".",".",".",".","8",".",".","7","9"},
};

int main() {
    makeBoxMap();
    SudokuBoard b1(rb1);
    assert(b1.isValidSudoku());
    SudokuBoard b2(rb2);
    assert(!b2.isValidSudoku());
    return 0;
}
