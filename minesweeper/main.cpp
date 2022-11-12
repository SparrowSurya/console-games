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
typedef unsigned short udtype_t;

/* signed datatype */
typedef int sdtype_t;

/* cell states */
typedef enum {
    HIDDEN,
    OPENED,
    FLAGGED
} state_t;

/* cell structure */
typedef struct {
    state_t state = HIDDEN;
    bool is_mine = false;
    udtype_t mines = 0;
} cell_t;

/* coord as r, c */
typedef struct {
    sdtype_t r = None;
    sdtype_t c = None;
} coord_t;

/* global variables */

cell_t **Mineboard = nullptr;
udtype_t Rows  = 0;
udtype_t Cols  = 0;
udtype_t Mines = 0;


/* functions declaration */

void generate();
bool resize(udtype_t, udtype_t);
void print();
udtype_t random(udtype_t);
coord_t *neighbour8(udtype_t, udtype_t);
void print_sep();
void print_cnt(udtype_t);


int main()
{
    srand(time(0));
    return 0;
}

bool resize(udtype_t rows, udtype_t cols)
{
    if (MINROW>rows && rows>MAXROW && MINCOL>cols && cols>MAXCOL) {
        return false;
    }
    if (Mineboard != nullptr) {
        delete[] Mineboard;
        Mineboard = nullptr;
    }
    Rows = rows;
    Cols = cols;
    Mineboard = new cell_t* [Rows];
    for (udtype_t i = 0; i < Cols; i++) {
        Mineboard[i] = new cell_t[Cols];
    }
    return true;
}

void print() {
    print_sep();
    for (udtype_t i = 0; i < Rows; i++) {
        print_cnt(i);
        print_sep();
    }    
}

/* 0 to end including end */
udtype_t random(udtype_t end) {
    return rand() % (end+1);
}

coord_t *neighbour8(udtype_t row, udtype_t col) {
    
}