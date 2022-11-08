#include "MineBoard.h"
#include <random>


int main()
{
    Minesweeper::MineBoard<char> board(4, 4);

    std::cout << "Rows: " << board.Rows() << "\n" << "Cols: " << board.Cols() << "\n";

    board.Print();

    return 0;
}