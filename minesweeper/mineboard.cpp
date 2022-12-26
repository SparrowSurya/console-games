#include <iostream>
#include <time.h>

#include "game.h"
#include "mineboard.h"


void Mineboard::Generate() {
    std::cout << "Generating... \n";
    short r, c;
    for (short i=0; i<this->mines; i++) {
        r = 1/* this->Random(0, this->rows) */; // testing purpose 
        c = 1/* this->Random(0, this->cols) */;
        this->board[r][c].is_mine = true;
        this->AdjustNeighbours(r, c, +1);
    }
    std::cout << "Generated! \n";
}

void Mineboard::Init() {
    std::cout << "Initialising... \n";
    if (this->board == nullptr) {
        this->board = new tile_t*[this->rows];
        for (short r=0; r<this->rows; r++) {
            this->board[r] = new tile_t[this->cols];
        }
    }
    std::cout << "Mineboard Initialised! \n";
}

void Mineboard::GetNeighbours(short r, short c, coord_t** empty) {
    std::cout << "Fetching neighbours coords from " << r << ',' << c << "... \n";
    short index = 0;
    short rel_dir[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    for (auto &d : rel_dir) {
        if (this->InBounds(r+d[1], c+d[0])) {
            empty[index] = new coord_t;
            (*empty)[index].r = r + d[0];
            (*empty)[index].c = c + d[1];
            index++;
        }
    }
    if (index<8) {
        empty[index] = nullptr;
    }
    std::cout << "Neighbours coords Fetched! \n";
}

void Mineboard::FillNeighbours(short r, short c, tile_t** empty) {
    std::cout << "Fetching neighbours from " << r << ',' << c << "... \n";
    short index = 0;
    short rel_dir[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    for (auto &d : rel_dir) {
        if (this->InBounds(r+d[1], c+d[0])) {
            std::cout << '(' << r+d[1] << ',' << c+d[0] << ')' << '\t';
            empty[index] = &(this->board[r+d[1]][c+d[0]]);
            std::cout << &(this->board[r+d[1]][c+d[0]]) << '\n';
            index++;
        }
    }
    if (index<8) {
        empty[index] = nullptr;
    }
    std::cout << "Neighbours Fetched! \n";
}

void Mineboard::AdjustNeighbours(short r, short c, short delta) {
    std::cout << "Adjusting (" << r << ',' << c << ")`s neighbours mines count by " << delta << '\n';
    tile_t *adj[8];
    this->FillNeighbours(r, c, adj); // works fine & also debugs correct address
    for (short i=0; ((i<8) & (adj[i]!=nullptr)) ; i++) {
        std::cout << "Index " << i << " Address " << &((*adj)[i]) << '\n'; // debugs & updates wrong tiles
        ((*adj)[i]).adj_mines += delta;
    }
    std::cout << "Neighbours Adjusted! \n";
}

void Mineboard::Ignore(short r, short c) {
    if (this->InBounds(r, c) && this->board[r][c].is_mine) {
        std::cout << "Ignoring the mine click at " << r << ',' << c << '\n';
        this->AdjustNeighbours(r, c, -1);
        do {
            r = this->Random(0, this->rows);
            c = this->Random(0, this->cols);
        } while (this->board[r][c].is_mine==true);
        this->AdjustNeighbours(r, c, 1);
        std::cout << "Ignored and moved mine to another unexplored location \n";
    }
}

void Mineboard::Reset() {
    std::cout << "Resetting... \n";
    std::cout << "Reset done! \n";    
}

void Mineboard::Resize(short rows, short cols, short mines = 0) {
    if (rows<MINROW || MAXROW<rows || cols<MINCOL || MAXCOL<cols) {
        return;
    }
    std::cout << "Resizing to " << rows << ',' << cols << ',' << mines << '\n';
    for (short r=0; r<this->rows; r++) {
        delete[] this->board[r];
    }
    delete[] this->board;
    this->board = nullptr;
    this->rows = rows;
    this->cols = cols;
    this->mines = mines==0? this->mines: mines;
    this->Init();
    std::cout << "Resized!";
}

void Mineboard::Expand(short r, short c) {
    std::cout << "Expanding from " << r << ',' << c << "... \n";
    std::cout << "Expansion done at " << r << ',' << c << '\n';  
}

void Mineboard::Pop(short r, short c) {
    if (this->InBounds(r, c)) {
        this->board[r][c].state = OPENED;
    }
}

void Mineboard::Flag(short r, short c) {
    if (this->InBounds(r, c) && this->board[r][c].state == CLOSED) {
        this->board[r][c].state = FLAGGED;
    }
}

void Mineboard::Unflag(short r, short c) {
    if (this->InBounds(r, c) && this->board[r][c].state == FLAGGED) {
        this->board[r][c].state = CLOSED;
    }
}

short Mineboard::Random(short min, short max) {
    return min + (rand() % max);
}

void Mineboard::PrintL() {
    std::cout << '+';
    for (short i=0; i<this->cols; i++) {
        std::cout << "---+";
    }
    std::cout << '\n';
}

void Mineboard::PrintM(short r) {
    std::cout << '|';
    for (short c=0; c<this->cols; c++) {
        if (this->board[r][c].state==CLOSED) {
            std::cout << (char)176 << (char)176 << (char)176 << '|';
        } else if (this->board[r][c].state==OPENED) {
            if (this->board[r][c].is_mine) {
                std::cout << ' ' << (char)254 << " |";
            } else if (this->board[r][c].adj_mines==0) {
                std::cout << "   |";
            } else {
                std::cout << ' ' << this->board[r][c].adj_mines << " |";
            }
        } else if (this->board[r][c].state==FLAGGED) {
            std::cout << ' ' << (char)35 << " |";
        } else {
            throw "Unexpected!  board tile has undefined state";
        }
    }
    std::cout << '\n';
}

void Mineboard::Print() {
    std::cout << '\n';
    this->PrintL();
    for (short r=0; r<this->rows; r++) {
        this->PrintM(r);
        this->PrintL();
    }
    std::cout << '\n';
}

short Mineboard::GetRows() {
    return this->rows;
}

short Mineboard::GetCols() {
    return this->cols;
}

short Mineboard::GetMines() {
    return this->mines;
}

inline bool Mineboard::InBounds(short r, short c) {
    return 0 <= r && r<this->rows && 0<=c && c<this->cols;
}

void Mineboard::Debug() {
    for (short i=0; i<this->rows; i++) {
        for (short j=0; j<this->cols; j++) {
            std::cout << &(this->board[i][j]) << ' ';
        }
        std::cout << '\n';
    }
}