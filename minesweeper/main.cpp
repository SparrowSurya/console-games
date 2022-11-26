#include <iostream>
#include <time.h>
#include <cstdlib>

/* settings */

#define MAXROW 8
#define MAXCOL 12
#define MINROW 2
#define MINCOL 2

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
    QUIT    = 'q', /* to quit the game */
    CMDS    = 'c', /* to get list of all commands with their uses */
    STATUS  = 'i', /* to know the status of game */
    POP     = 'x', /* to hit a tile */
    FLAG    = 'f', /* to flag athe tile */
    UNFLAG  = 'u', /* to unflag a tile */
    EXPAND  = 'e', /* to expand recursively on cells who have mines around them detected */
    NEW     = 'n', /* to start new match*/
    RESIZE  = 'r'  /* to resize the mineboard dimensions */
} cmd;

/* from character digit to number ; skips invalid digit character */
num_t digitise(std::string __s) {
    num_t num=0, tmp;
    for (auto c: __s) {
       tmp = (num_t) c;
       if (40<=tmp && tmp<=57) {
            num *= 10;
            num += tmp-40;
       }
    }
    return num;
}

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

/* fills adjacent tiles in given parameter */
void get_adj_tiles(num_t r, num_t c, tile_t** adj_tiles) {
    short rel_dir[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    num_t index = 0;
    
    for (auto &d : rel_dir) {
        if (0 <= (r + d[0]) && (r + d[0]) < Rows && 0 <= (c + d[1]) && (c + d[1]) < Cols) {
            adj_tiles[index] = &(Mineboard[r + d[0]][c + d[1]]);
            index++;
        }
    }
    if (index<7) {
        adj_tiles[index] = nullptr;
    }
}

/* fills adjacent tiles coord in given parameter */
void get_adj_tiles(num_t r, num_t c, coord_t** adj_tiles) {
    short rel_dir[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    num_t index = 0;
    
    for (auto &d : rel_dir) {
        if (0 <= (r + d[0]) && (r + d[0]) < Rows && 0 <= (c + d[1]) && (c + d[1]) < Cols) {
            (*adj_tiles[index]).r = r + d[0];
            (*adj_tiles[index]).c = c + d[1];
            index++;
        }
    }
    if (index<7) {
        adj_tiles[index] = nullptr;
    }
}

/* changes mines amount in adjacent tiles */
void modify_adjacent_mines(num_t r, num_t c, short amount) {
    tile_t** adj_tiles;
    get_adj_tiles(r, c, adj_tiles);

    for (num_t i=0; i<8; i++) {
        if (adj_tiles[i] == nullptr) {
            break;
        } else {
            (*adj_tiles[i]).mines += amount;
        }
    }
}

/* to generate mineboard */
void generate() {
    num_t m = Mines, r, c;
    std::cout << "Mines at: ";
    while (m > 0) {
        r = random(Rows - 1);
        c = random(Cols - 1);

        if (Mineboard[r][c].is_mine == false) {
            Mineboard[r][c].is_mine = true;
            modify_adjacent_mines(r, c, 1);
            m--;
            std::cout << '('<< r << ", " << c << ") ";
        }
    }
    std::cout << std::endl;
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
    std::cout   << std::endl;
    std::cout   << "Commands: \n\n" <<
    cmd::QUIT   << " - to quit the program  \n" <<
    cmd::CMDS   << " - displays all the commands  \n" <<
    cmd::NEW    << " - starts new game  \n" <<
    cmd::STATUS << " - show status of game  \n" <<
    cmd::POP    << " rows cols - breks the tilw at row col  \n" <<
    cmd::FLAG   << " rows cols - flags the tilw at row col  \n" <<
    cmd::UNFLAG << " rows cols - removes flag on tile at row col  \n" <<
    cmd::EXPAND << " rows cols - work on tiles whose all neighbour mines are detected (works recursively)  \n" <<
    cmd::RESIZE << " rows cols mines - resizes the board (use 0 for param to use previous settings)  \n" <<
    std::endl;
}

/* to break a tile */
bool pop(num_t r, num_t c) {
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
    get_adj_tiles(r, c, neighbours);
    if (is_expansion_possible(r, c, neighbours)) {
        for (num_t i=0; i<8; i++) {
            if (Mineboard[r][c].state == CLOSED) {
                Mineboard[r][c].state = OPENED;
                expand((*neighbours)[i].r, (*neighbours)[i].c);
            }
        }
    }
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

    std::string cmd, token;
    num_t bindex, t; 
    void *buffer[4] {new char, new std::string, new std::string, new std::string};

    srand(time(0)); // random seed for each program instance
    std::cout << '\n' << "--------------------Minesweeper--------------------" << "\n\n";

    resize(7, 10, 10);
    generate();

    /* gameloop */
    while (1) {
        print();
        std::cout << ">>> ";
        std::cin >> cmd;

        /* command breakdown */
        t = 0, bindex = 0;
        while (1) {
            if (cmd[t] == ' ') {
                std::cout << "Token: " << token << '\n';
                *(std::string*)buffer[bindex] = token;
                token.clear();
            } else if (cmd[t] == '\0') {
                std::cout << "Token: " << token << '\n';
                *(std::string*)buffer[bindex] = token;
                break;
            } else {
                token.push_back(cmd[t]);
            }
            t++;
        }
        
        /* actions */
        switch ((char) toupper(cmd[0])) {
            case (cmd::POP): {
                pop(digitise(*(std::string*)buffer[1]), digitise(*(std::string*)buffer[2]));
            } break;

            case (cmd::FLAG): {
                flag(digitise(*(std::string*)buffer[1]), digitise(*(std::string*)buffer[2]));
            } break;

            case (cmd::UNFLAG): {
                unflag(digitise(*(std::string*)buffer[1]), digitise(*(std::string*)buffer[2]));
            } break;

            case (cmd::EXPAND): {
                expand(digitise(*(std::string*)buffer[1]), digitise(*(std::string*)buffer[2]));
            } break;

            case (cmd::NEW): {
                newgame();
            } break;

            case (cmd::RESIZE): {
                resize(digitise(*(std::string*)buffer[1]), digitise(*(std::string*)buffer[2]), digitise(*(std::string*)buffer[3]));
                std::cout << "\n----------BOARD-RESIZED----------\n";
                newgame();
            } break;

            case (cmd::CMDS): {
                info();
            } break;

            case (cmd::QUIT): {
                return 0;
            } break;
        }
    }
    return 0;
}

int main() {
    return Minesweeper();
}
