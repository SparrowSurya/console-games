#include "MineBoard.h"


int main()
{
    Minesweeper::MineBoard<char> board(4, 4, 5);

    std::cout << "Rows: " << board.Rows() << "\n" << "Cols: " << board.Cols() << "\n";

    board.Print();

    return 0;
}