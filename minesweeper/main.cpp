#include <iostream>
#include <time.h>
#include <cstdlib>
#include <limits.h>

/* settings */

#define MAXROW 8
#define MINROW 2
#define MAXCOL 12
#define MINCOL 2
#define NULL_COORD USHRT_MAX
#define NULL_MINE  10

/* unsigned datatype */
typedef unsigned short num_t;

/* tile states */
typedef enum {
    CLOSED,
    OPENED,
    FLAGGED
} state_t;

/* tile structure */
typedef struct {
    state_t state = CLOSED;
    bool is_mine = false;
    num_t mines = 0;
} tile_t;

/* coord struct having r, c info */
typedef struct {
    num_t r;
    num_t c;
}coord_t;

/* global variables */

tile_t **Mineboard = nullptr;
num_t Rows = 0;
num_t Cols = 0;
num_t Mines = 0;

/* various commands */
typedef enum {
    QUIT    = 'Q', /* to quit the game */
    CMDS    = 'C', /* to get list of all commands with their uses */
    STATUS  = 'I', /* to know the status of game */
    POP     = 'X', /* to hit a tile */
    FLAG    = 'F', /* to flag athe tile */
    UNFLAG  = 'U', /* to unflag a tile */
    EXPAND  = 'E', /* to expand recursively on cells who have mines around them detected */
    NEW     = 'N', /* to start new match*/
    RESIZE  = 'R'  /* to resize the mineboard dimensions */
} cmd;


/* sub-function of print to print single row elemnts of mineboard based on their data */
void print_cnt(num_t r) {
    std::cout << '|';
    for (num_t c = 0; c < Cols; c++) {

        if (Mineboard[r][c].state == CLOSED) {
            std::cout << (char)176 << (char)176 << (char)176 << '|';

        } else if (Mineboard[r][c].state == OPENED) {

            if (Mineboard[r][c].is_mine) {
                std::cout << ' ' << (char)254 << ' ' << '|';
            } else if (Mineboard[r][c].mines == 0) {
                std::cout << "   |";
            } else {
                std::cout << ' ' << Mineboard[r][c].mines << ' ' << '|';
            }
        }
        else if (Mineboard[r][c].state == FLAGGED) {
            std::cout << ' ' << (char)35 << ' ' << '|';
        } else {
            throw "Unexpected cause for board tile has undefined state";
        }
    }
    std::cout << '\n';
}

/* sub-function of print function to print single seperating row */
void print_sep() {
    std::cout << '+';
    for (num_t i = 0; i < Cols; i++) {
        std::cout << "---+";
    }
    std::cout << '\n';
}

/* outputs mineboard to console */
void print() {
    std::cout << '\n';
    print_sep();
    for (num_t i = 0; i < Rows; i++) {
        print_cnt(i);
        print_sep();
    }
    std::cout << '\n';
}

/* returns random number excluding end */
num_t random(num_t end) {
    return rand() % end;
}

/* erases all data to default in mineboard */
void reset() {
    for (num_t r = 0; r < Rows; r++) {
        for (num_t c = 0; c < Cols; c++) {
            Mineboard[r][c].is_mine = false;
            Mineboard[r][c].mines = 0;
            Mineboard[r][c].state = CLOSED;
        }
    }
}

/* returns adjacent tiles */
tile_t* get_adj_tiles(num_t r, num_t c, tile_t adj_tiles[8]) {
    short rel_dir[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    num_t index = 0;
    
    for (auto &d : rel_dir) {
        if (0 <= (r + d[0]) && (r + d[0]) < Rows && 0 <= (c + d[1]) && (c + d[1]) < Cols) {
            adj_tiles[index] = Mineboard[r + d[0]][c + d[1]];
            index++;
        }
    }
    return adj_tiles;
}

/* returns adjacent tiles coord */
coord_t* get_adj_tiles(num_t r, num_t c, coord_t adj_tiles[8]) {
    short rel_dir[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    num_t index = 0;
    
    for (auto &d : rel_dir) {
        if (0 <= (r + d[0]) && (r + d[0]) < Rows && 0 <= (c + d[1]) && (c + d[1]) < Cols) {
            (adj_tiles[index]).r = r + d[0];
            (adj_tiles[index]).c = c + d[1];
            index++;
        }
    }
    if (index<7) {
        adj_tiles[index].r = 10;
        adj_tiles[index].c = 99;
    }
    return adj_tiles;
}

/* changes mines amount in adjacent tiles */
void modify_adjacent_mines(num_t r, num_t c, short amount) {
    tile_t *adj_tiles[8];
    adj_tiles = get_adj_tiles(r, c, adj_tiles);

    for (num_t i=0; i<8; i++) {
        (*adj_tiles)[i].mines += amount;
    }
}

/* to generate mineboard */
void generate() {
    num_t m = Mines, r, c;

    while (m > 0) {
        r = random(Rows - 1);
        c = random(Cols - 1);

        if (Mineboard[r][c].is_mine == false) {
            Mineboard[r][c].is_mine = true;
            modify_adjacent_mines(r, c, 1);
            m--;
        }
    }
}

/* condition check for recursive expansion */
bool is_expansion_possible(num_t r, num_t c, coord_t* adj_tiles[8]) {
    num_t flags = 0;
    if (Mineboard[r][c].state == CLOSED) {
        for (num_t i = 0; i < 8; i++) {
            if (adj_tiles[i] == nullptr) {
                break;
            }
            if (Mineboard[(*adj_tiles)[i].r][(*adj_tiles)[i].c].state == FLAGGED) {
                flags++;
            }
        }

        if (flags == Mineboard[r][c].mines) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

/* prevents the first selected tile to be mine */
void first_guess_wrong(num_t r, num_t c) {
    modify_adjacent_mines(r, c, -1);
    do {
        r = random(Rows);
        c = random(Cols);
    } while (Mineboard[r][c].is_mine == true);
    modify_adjacent_mines(r, c, +1);
}


void info() {
    std::cout << std::endl;
    std::cout << "Commands: \n\n" <<
    "Q - to quit the program  \n" <<
    "C - displays all the commands  \n" <<
    "N - starts new game  \n" <<
    "S - show status of game  \n" <<
    "X rows cols - breks the tilw at row col  \n" <<
    "F rows cols - flags the tilw at row col  \n" <<
    "U rows cols - removes flag on tile at row col  \n" <<
    "E rows cols - work on tiles whose all neighbour mines are detected (works recursively)  \n" <<
    "R rows cols mines - resizes the board (use 0 for param to use previous settings)  \n" <<
    std::endl;
}

/* to break a tile */
bool press(num_t r, num_t c) {
    if (0 <= r && r < Rows && 0 <= c && c < Cols && Mineboard[r][c].state == CLOSED) {
        Mineboard[r][c].state = OPENED;
        return true;
    }
    return false;
}

bool flag(num_t r, num_t c) {
    if (0 <= r && r < Rows && 0 <= c && c < Cols && Mineboard[r][c].state == CLOSED) {
        Mineboard[r][c].state = FLAGGED;
        return true;
    }
    return false;
}

bool unflag(num_t r, num_t c) {
    if (0 <= r && r < Rows && 0 <= c && c < Cols && Mineboard[r][c].state == FLAGGED) {
        Mineboard[r][c].state = CLOSED;
        return true;
    }
    return false;
}

void expand(num_t r, num_t c) {
    coord_t *neighbours[8];
    *neighbours = get_adj_tiles(r, c, neighbours);
    if (is_expansion_possible(r, c, neighbours)) {
        for (num_t i=0; i<8; i++) {
            if (Mineboard[r][c].state == CLOSED) {
                Mineboard[r][c].state = OPENED;
                expand((*neighbours)[i].r, (*neighbours)[i].c);
            }
        }
    }
    delete[] neighbours;
}

void newgame() {
    reset();
    generate();
    std::cout << "\n----------NEW-GAME----------\n";
}

bool resize(num_t rows, num_t cols, num_t mines) {
    if (MINROW > rows && rows > MAXROW && MINCOL > cols && cols > MAXCOL) {
        return false;
    }
    if (Mineboard != nullptr) {
        delete[] Mineboard;
        Mineboard = nullptr;
    }

    Rows = rows;
    Cols = cols;
    Mines = mines;
    Mineboard = new tile_t *[Rows];

    for (num_t i = 0; i < Cols; i++) {
        Mineboard[i] = new tile_t[Cols];
    }
    return true;
}

bool endgame() {
    return false;
}

int Minesweeper() {

    std::cout << '\n' << "--------------------Minesweeper--------------------" << "\n\n";

    // initializations
    srand(time(0)); // random seed for each program instance
    std::string cmd;
    char cmd_in; 
    num_t row_in, col_in, mines_in;

    std::cout << "Enter the size of row column and no of mines to place: \n>>> ";
    std::cin >> row_in >> col_in >> mines_in;

    resize(row_in, col_in, mines_in);
    generate();

    // game starts
    while (1) {
        print();
        std::cout << ">>> ";
        std::cin >> cmd_in;

        // parser
        

        switch (cmd_in) {
            case (cmd::POP): {
                // std::cin >> row_in >> col_in;
                press(row_in, col_in);
            } break;

            case (cmd::FLAG): {
                // std::cin >> row_in >> col_in;
                flag(row_in, col_in);
            } break;

            case (cmd::UNFLAG): {
                // std::cin >> row_in >> col_in;
                unflag(row_in, col_in);
            } break;

            case (cmd::EXPAND): {
                // std::cin >> row_in >> col_in;
                expand(row_in, col_in);
            } break;

            case (cmd::NEW): {
                newgame();
            } break;

            case (cmd::RESIZE): {
                // std::cin >> row_in >> col_in >> mines_in;
                resize(row_in, col_in, mines_in);
                std::cout << "\n----------BOARD-RESIZED----------\n";
                newgame();
            } break;

            case (cmd::CMDS): {
                info();
            } break;

            case (cmd::QUIT): {
                return 0;
            } break;

            default: {
                std::cout << "Invalid Command! \n";
            }
        }
    }
    return 1;
}

int main() {
    return Minesweeper();
}
/*
pending:
recursive expansion
recursive expansion on empty tiles
*/