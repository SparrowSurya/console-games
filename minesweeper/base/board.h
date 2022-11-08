#ifndef _BOARD_H
#define _BOARD_H 1

#include "utils.h"

template <typename _S, typename _T>
class _board
{
private:
    _S **board = nullptr;
    _T rows;
    _T cols;

public:
    _board(_T rows, _T cols)
    {
        this->rows = rows;
        this->cols = cols;
    }

    ~_board()
    {
        delete cols;
        delete rows;
        delete board;
    }

    /* return item in the given position */
    _S item(_S row, _S col)
    {
        return board[row][col];
    }

    /* returns up, down, left, right coordinates as 2d array of row & col
    for row, col = -1, -1 it means there no more neighbours */
    _coord<_T> *neighbours_4(_T row, _T col, _T maxr, _T maxc)
    {
        _T index = 0;
        _T neighbour = new _T **[4][2]
        {
            {-1, -1}, {-1, -1}, {-1, -1}, { -1, -1 }
        };
        _T _dir[4][2] = {
            {1, 0}, {0, 1}, {-1, 0}, {-1, 0}};

        for (auto d : _dir)
        {
            if (0 <= row + d[0] && row + d[0] <= maxr && 0 <= col + d[1] && col + d[1] <= maxc)
            {
                neighbour[index][0] = row + d[0];
                neighbour[index][1] = col + d[1];
                index++;
            }
        }

        return &neighbour;
    }

    /* returns up, down, left, right, upper left, upper right, lower left, lower right coordinates as 2d array of row & col
    for row, col = -1, -1 it means there no more neighbours */
    _coord<_T> *neighbours_4(_T row, _T col, _T maxr, _T maxc)
    {
        _T index = 0;
        _T neighbour = new _T **[8][2]
        {
            {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, { -1, -1 }
        };
        _T _dir[8][2] = {
            {1, 0}, {0, 1}, {-1, 0}, {-1, 0}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

        for (auto d : _dir)
        {
            if (0 <= row + d[0] && row + d[0] <= maxr && 0 <= col + d[1] && col + d[1] <= maxc)
            {
                neighbour[index][0] = row + d[0];
                neighbour[index][1] = col + d[1];
                index++;
            }
        }

        return &neighbour;
    }
}; // class _board

#endif