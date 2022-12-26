#ifndef _MINESWEEPER_H_
#define _MINESWEEPER_H_ 1

/* settings */

#define MAXROW 8
#define MAXCOL 12
#define MINROW 2
#define MINCOL 2

/* tile state */
typedef enum {
    CLOSED,
    OPENED,
    FLAGGED
} state_t;

/* tile info */
typedef struct {
    state_t state = CLOSED;
    bool is_mine = false;
    short adj_mines = 0;
} tile_t;

/* struct to hold neighbours */
typedef struct {
    tile_t* adj[8];
} adj_t;

/* coord having row, column info */
typedef struct {
    short r, c;
} coord_t;


#endif
