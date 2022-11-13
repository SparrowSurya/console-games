#include <iostream>
#include <time.h>
#include <cstdlib>

/* settings */

#define MAXROW 8
#define MINROW 2
#define MAXCOL 12
#define MINCOL 2
#define None -1

/* unsigned datatype */
typedef unsigned short dtype_t;

/* cell states */
typedef enum
{
    HIDDEN,
    OPENED,
    FLAGGED
} state_t;

/* cell structure */
typedef struct
{
    state_t state = HIDDEN;
    bool is_mine = false;
    dtype_t mines = 0;
} cell_t;

/* global variables */

cell_t **Mineboard = nullptr;
dtype_t Rows = 0;
dtype_t Cols = 0;
dtype_t Mines = 0;

/* functions declaration */

void generate();                          /* generates mineboard mines */
void reset();                             /* erases all data to default in mineboard */
void increment_mines(dtype_t, dtype_t);   /* increments mines count in neighbours */
bool resize(dtype_t, dtype_t);            /* resizes the mineboard */
void first_guess_wrong(dtype_t, dtype_t); /* prevents the first selected cell to be mine */
dtype_t random(dtype_t);                  /* returns random number including end */
cell_t **neighbour8(dtype_t, dtype_t);    /* returns the eight neighbour cells */
void print();                             /* outputs mineboard to console */
void print_sep();                         /* sub-function of print function to print single seperating row */
void print_cnt(dtype_t);                  /* sub-function of print to print single row elemnts of mineboard based on their data */

int main() // MAIN
{
    srand(time(0)); // random seed for each program instance

    return 0;
}

/* to generate mineboard */
void generate()
{
    dtype_t m = Mines, r, c;

    while (m) // placing mines
    {
        r = random(Rows - 1);
        c = random(Cols - 1);

        if (Mineboard[r][c].is_mine == false)
        {
            Mineboard[r][c].is_mine = true;
            increment_mines(r, c);
            m--;
        }
    }
}

/* increments mines count in neighbours */
void increment_mines(dtype_t r, dtype_t c)
{
    cell_t **neighbour = neighbour8(r, c);
    for (auto n : neighbour)
    {
        if (n == nullptr)
        {
            break;
        }
        n.mines++;
    }
}

/* resizes the mineboard */
bool resize(dtype_t rows, dtype_t cols)
{
    if (MINROW > rows && rows > MAXROW && MINCOL > cols && cols > MAXCOL)
    {
        return false;
    }
    if (Mineboard != nullptr)
    {
        delete[] Mineboard;
        Mineboard = nullptr;
    }
    Rows = rows;
    Cols = cols;
    Mineboard = new cell_t *[Rows];
    for (dtype_t i = 0; i < Cols; i++)
    {
        Mineboard[i] = new cell_t[Cols];
    }
    return true;
}

/* erases all data to default in mineboard */
void reset()
{
    for (dtype_t r = 0; r < Rows; r++)
    {
        for (dtype_t c = 0; c < Cols; c++)
        {
            Mineboard[r][c].is_mine = false;
            Mineboard[r][c].mines = 0;
            Mineboard[r][c].state = HIDDEN;
        }
    }
}

/* outputs mineboard to console */
void print()
{
    std::cout << '\n';
    print_sep();
    for (dtype_t i = 0; i < Rows; i++)
    {
        print_cnt(i);
        print_sep();
    }
    std::cout << '\n';
}

/* returns random number including end */
dtype_t random(dtype_t end)
{
    return rand() % (end + 1);
}

/* returns the neighbour cell coordinates */
cell_t **neighbour8(dtype_t r, dtype_t c)
{
    cell_t **neighbour = new cell_t *[8]();
    dtype_t i = 0;
    short rel_dir[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

    for (auto d : rel_dir)
    {
        if (MINROW <= r + d[0] && r + d[0] <= MAXROW && MINCOL <= c + d[1] && c + d[1] <= MAXCOL)
        {
            neighbour[i] = &(Mineboard[r + d[0]][c + d[1]]);
            i++;
        }
    }
    return neighbour;
}

/* sub-function of print to print single row elemnts of mineboard based on their data */
void print_cnt(dtype_t r)
{
    std::cout << '|';
    for (dtype_t c = 0; c < Cols; c++)
    {
        if (Mineboard[r][c].state == HIDDEN)
        {
            std::cout << (char)176 << (char)176 << (char)176 << '|';
        }
        else if (Mineboard[r][c].state == OPENED)
        {
            if (Mineboard[r][c].mines == 0)
            {
                std::cout << "   ";
            }
            else
            {
                std::cout << ' ' << Mineboard[r][c].mines << ' ' << '|';
            }
        }
        else if (Mineboard[r][c].state == FLAGGED)
        {
            std::cout << ' ' << (char)254 << ' ' << '|';
        }
        else
        {
            throw "Unexpected cause for board cell has undefined state";
        }
    }
    std::cout << '\n';
}

/* sub-function of print function to print single seperating row */
void print_sep()
{
    std::cout << '+';
    for (dtype_t i = 0; i < Cols; i++)
    {
        std::cout << "---+";
    }
    std::cout << '\n';
}