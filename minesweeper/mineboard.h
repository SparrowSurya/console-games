#ifndef _MINEBOARD_H_
#define _MINEBOARD_H_ 1

#include "game.h"

class Mineboard {
    private:
        tile_t** board = nullptr;
        short rows  = 7;
        short cols  = 10;
        short mines = 10;

        void Generate();
        void Ignore(short, short);
        void PrintL();
        void PrintR(short n);
        short Random(short, short);
        void AdjustNeighbours(short, short, short);
        bool IsExpandable(short, short);
        void FillNeighbours(short, short, struct adj<tile_t>*);
        void FetchNeighboursLoc(short, short, struct adj<coord_t>*);
        void FreeNeighboursLoc(struct adj<coord_t>*);
    
    public:
        void Init();
        Mineboard() = default;
        void Resize(short, short, short);
        void Expand(short, short);
        void Flag(short, short);
        void Unflag(short, short);
        void Pop(short, short);
        void Print();
        void Reset();
        short GetRows();
        short GetCols();
        short GetMines();
        inline bool InBounds(short, short);
};

#endif