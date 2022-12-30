#include <iostream>
#include <time.h>

#include "game.h"
#include "mineboard.h"

/* Places mines inside the board */
void Mineboard::Generate() {
    short r, c;
    for (short i=0; i<this->mines; i++) {
        r = this->Random(0, this->rows);
        c = this->Random(0, this->cols);
        this->board[r][c].is_mine = true;
        this->AdjustNeighbours(r, c, +1);
    }
}

/* If board dont exists then creates and generate it */
void Mineboard::Init() {
    if (this->board != nullptr) {
        return;
    }
    this->board = new tile_t*[this->rows];
    for (short r=0; r<this->rows; r++) {
        this->board[r] = new tile_t[this->cols];
    }
    this->Generate();
}

/* writes the coordinates of neighbous tiles 
 * writes nullptr if less neighbours
 * neighbour size 8
 * uses dynamic allocation
 */
void Mineboard::FetchNeighboursLoc(short r, short c, struct adj<coord_t>* empty) {
    short index = 0;
    short rel_dir_rc[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    for (auto &d : rel_dir_rc) {
        if (this->InBounds(r+d[1], c+d[0])) {
            (*empty).adj[index] = new coord_t;
            (*empty).adj[index]->r = r+d[1];
            (*empty).adj[index]->c = c+d[0];
            index++;
        }
    }
    if (index<8) {
        (*empty).adj[index] = nullptr;
    }
}

/* writes address of neighbours
 * writes nullptr if less neighbours
 */
void Mineboard::FillNeighbours(short r, short c, struct adj<tile_t>* empty) {
    short i = 0;
    short rel_dir_rc[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}
    };
    for (auto &d : rel_dir_rc) {
        if (this->InBounds(r+d[1], c+d[0])) {
            (*empty).adj[i] = &(this->board[r+d[1]][c+d[0]]);
            i++;
        }
    }
    if (i<8) {
        (*empty).adj[i] = nullptr;
    }
}

/* adds the delta value to neighbour mines count */
void Mineboard::AdjustNeighbours(short r, short c, short delta) {
    struct adj<tile_t> adj;
    this->FillNeighbours(r, c, &adj);
    for (short i=0; ((i<8) & (adj.adj[i]!=nullptr)) ; i++) {
        (*adj.adj[i]).adj_mines += delta;
    }
}

/* If mine at r,c exists then moves the mine to unexplored tile
 * random selection approach
 * caution: goes to infinite loop if no suck tile exists
*/
void Mineboard::Ignore(short r, short c) {
    if (!this->InBounds(r, c) || !this->board[r][c].is_mine) {
        return;
    }
    this->AdjustNeighbours(r, c, -1);
    do {
        r = this->Random(0, this->rows);
        c = this->Random(0, this->cols);
    } while (this->board[r][c].is_mine==true);
    this->AdjustNeighbours(r, c, 1);
}

/* restores the board to default values and generates */
void Mineboard::Reset() {
    for (short r=0; r<this->GetRows(); r++) {
        for (short c=0; c<this->GetCols(); c++) {
            this->board[r][c].adj_mines = 0;
            this->board[r][c].is_mine = false;
            this->board[r][c].state = CLOSED;
        }
    }
    this->Generate();
}

/* frees the board and creates new allocation and generates */
void Mineboard::Resize(short rows, short cols, short mines = 0) {
    if (rows<MINROW || MAXROW<rows || cols<MINCOL || MAXCOL<cols) {
        return;
    }
    for (short r=0; r<this->rows; r++) {
        delete[] this->board[r];
    }
    delete[] this->board;
    this->board = nullptr;
    this->rows = rows;
    this->cols = cols;
    this->mines = mines==0? this->mines: mines;
    this->Init();
}

/* whether tile is subject to expansion */
bool Mineboard::IsExpandable(short r, short c) {
    short flags = 0;
    struct adj<tile_t> adj;
    this->FillNeighbours(r, c, &adj);
    for (short i=0; ((i<8) & (adj.adj[i]!=nullptr)) ; i++) {
        if ((*adj.adj[i]).state == FLAGGED) {
            flags++;
        }
    }
    return flags == this->board[r][c].adj_mines;
}

/* if r,c tile hasa no adjacent mines then Pops all neighbour tiles with same property
 * does same for all neighbours recursively
 * frees allocated memory
 */
void Mineboard::Expand(short r, short c) {
    if (!this->IsExpandable(r, c)) {
        return;
    }
    this->board[r][c].state = OPENED;
    struct adj<coord_t> adj;
    this->FetchNeighboursLoc(r, c, &adj);
    for (short i=0; ((i<8) & (adj.adj[i]!=nullptr)) ; i++) {
        if (this->board[adj.adj[i]->r][adj.adj[i]->c].state == CLOSED) {
            this->Expand(adj.adj[i]->r, adj.adj[i]->c);
        }
    }
    this->FreeNeighboursLoc(&adj);
}

/* free the occupied memory used in expand method */
void Mineboard::FreeNeighboursLoc(struct adj<coord_t>* empty) {
    for (short i=0; ((i<8) & ((*empty).adj[i]!=nullptr)) ; i++) {
        delete (*empty).adj[i];
    }    
}

/* opens the tile at r,c */
void Mineboard::Pop(short r, short c) {
    if (!this->InBounds(r, c)) {
        return;
    }
    if (this->board[r][c].adj_mines == 0) {
        this->Expand(r, c);
    } else {
        this->board[r][c].state = OPENED;
    }
}

/* flags the closed tile at r,c */
void Mineboard::Flag(short r, short c) {
    if (this->InBounds(r, c) && this->board[r][c].state == CLOSED) {
        this->board[r][c].state = FLAGGED;
    }
}

/* unflags the opened tile at r,c */
void Mineboard::Unflag(short r, short c) {
    if (this->InBounds(r, c) && this->board[r][c].state == FLAGGED) {
        this->board[r][c].state = CLOSED;
    }
}

/* returns random int including min but not max */
short Mineboard::Random(short min, short max) {
    return min + (rand() % max);
}

/* prints the line seperator */
void Mineboard::PrintL() {
    std::cout << '+';
    for (short i=0; i<this->cols; i++) {
        std::cout << "---+";
    }
    std::cout << '\n';
}

/* prints the row */
void Mineboard::PrintR(short r) {
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

/* prints the board */
void Mineboard::Print() {
    std::cout << '\n';
    this->PrintL();
    for (short r=0; r<this->rows; r++) {
        this->PrintR(r);
        this->PrintL();
    }
    std::cout << '\n';
}

/* returns board row count */
short Mineboard::GetRows() {
    return this->rows;
}

/* returns board column count */
short Mineboard::GetCols() {
    return this->cols;
}

/* returns board mine count */
short Mineboard::GetMines() {
    return this->mines;
}

/* if r,c lies in board */
inline bool Mineboard::InBounds(short r, short c) {
    return 0 <= r && r<this->rows && 0<=c && c<this->cols;
}

