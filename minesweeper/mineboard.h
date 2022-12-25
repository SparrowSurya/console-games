#ifndef _MINEBOARD_H_
#define _MINEBOARD_H_ 1

#include "game.h"

class Mineboard {
    // private:
    public:
        tile_t** board = nullptr;
        short rows  = 7;
        short cols  = 10;
        short mines = 1;

        short GetRows();
        short GetCols();
        short GetMines();
        void Generate();
        void Ignore(short, short);
        void PrintL();
        void PrintM(short n);
        short Random(short, short);
        void Init();
        void AdjustNeighbours(short, short, short);
        void Debug();
    
        Mineboard() = default;
        Mineboard(short, short, short);
        void Resize(short, short, short);
        void FillNeighbours(short, short, tile_t**);
        void GetNeighbours(short, short, coord_t**);
        void Expand(short, short);
        void Flag(short, short);
        void Unflag(short, short);
        void Pop(short, short);
        void Print();
        void Reset();
        inline bool InBounds(short, short);
};

#endif