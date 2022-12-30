#ifndef _MINEBOARD_H_
#define _MINEBOARD_H_ 1

#include "game.h"

class Mineboard {
    private:
        tile_t** board = nullptr;
        short rows  = 7;
        short cols  = 10;
        short mines = 1;
        bool explosion = false;
        short tile_broken = 0;
        short tile_flagged = 0;

        void Generate();
        void Ignore(short, short);
        void PrintL();
        void PrintR(short, bool);
        short Random(short, short);
        void AdjustNeighbours(short, short, short);
        bool IsExpandable(short, short);
        void FillNeighbours(short, short, struct adj<tile_t>*);
        void FetchNeighboursLoc(short, short, struct adj<coord_t>*);
        void FreeNeighboursLoc(struct adj<coord_t>*);
        void PopOne(short, short);

    public:
        void Init();
        Mineboard() = default;
        bool Resize(short, short, short);
        bool Expand(short, short);
        bool Flag(short, short);
        bool Unflag(short, short);
        bool Pop(short, short);
        void Print(bool);
        void Reset();
        bool Demined();
        short GetRows();
        short GetCols();
        short GetMines();
        short GetBrokenTiles();
        short GetFlaggedTiles();
        bool Explosion();
        inline bool InBounds(short, short);
};

#endif