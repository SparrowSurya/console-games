#ifndef _MINEBOARD_H
#define _MINEBOARD_H 1

#include <iostream>
#include "random.h"
#include "utils.h"
#include "settings.h"

/* game namespace */
namespace Minesweeper
{

    /* mineboard object for handling board */
    template <typename _T>
    class MineBoard
    {
    private:
        _cell **board = nullptr;
        uint16_t rows;
        uint16_t cols;
        uint16_t mines = 0;

        /* prints a row containing board values */
        void _print_container(uint16_t n)
        {
            std::cout << (char)186;

            for (uint16_t i = 0; i < this->cols; i++)
            {

                /* requires two different wyas to stdout the cell */
                std::cout << (char)176 << (char)176 << (char)176 << (char)186;
            }
            std::cout << '\n';
        }

        /* prints a row of seperator seperating rows */
        void _print_seperator()
        {
            std::cout << '+';
            for (uint16_t i = 0; i < this->cols; i++)
            {
                std::cout << "---+";
            }
            std::cout << '\n';
        }

        /* internal function to resize board; dont checks for dimensions constraints */
        void _resize()
        {
            if (board != nullptr)
            {
                delete board;
                board = nullptr;
            }
            board = new _cell* [this->rows];
            for (int i = 0; i < this->cols; i++)
            {
                board[i] = new _cell[this->cols];
            }
        }

        /* to generte board mines */
        void _gen()
        {
            uint16_t n = 0, r, c;

            while (true)
            {
                r = (uint16_t)random(this->rows + 1);
                c = (uint16_t)random(this->cols + 1);

                if (this->board[r][c].ismine == false)
                {
                    this->board[r][c].ismine = true;
                    n++;
                }
                if (n >= this->mines)
                {
                    break;
                }
            }

            for (uint16_t i = 0; i < this->rows; i++)
            {
                for (uint16_t j = 0; j < this->cols; j++)
                {
                    // this->board[i][j].
                }
            }
        }

    public:
        /* parameters rows and cols are subject to constraint */
        MineBoard(uint16_t rows, uint16_t cols, uint16_t mines)
        {
            if (MIN_ROW <= rows && rows <= MAX_ROW && MIN_COL <= cols && cols <= MAX_COL)
            {
                this->rows = rows;
                this->cols = cols;
                this->mines = mines;
                random_init();
            }
            else
            {
                throw("Mineboard initialization with dimensions out of bound \n");
            }
        }

        /* returns no of rows in board */
        uint16_t Rows()
        {
            return this->rows;
        }

        /* returns no of cols in board */
        uint16_t Cols()
        {
            return this->cols;
        }

        /* prints board */
        void Print()
        {
            this->_print_seperator();
            for (short i = 0; i < this->rows; i++)
            {
                this->_print_container(i);
                this->_print_seperator();
            }
        }

        /* resizes the board; dimensions are subject to constraint */
        bool Resize(uint16_t rows, uint16_t cols)
        {
            if (MIN_ROW <= rows && rows <= MAX_ROW && MIN_COL <= cols && cols <= MAX_COL)
            {
                this->rows = rows;
                this->cols = cols;
                this->_resize();
                return true;
            }
            return false;
        }
    };

} // namespace Minesweeper

#endif