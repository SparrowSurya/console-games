#ifndef _UTILS_H
#define _UTILS_H 1

/* struct to store row col coordinates */
template <typename _T>
struct _coord
{
    _T r;
    _T c;
};

/* various states for cell*/
typedef enum
{
    UNOPENED,
    OPENED,
    FLAGED
} _state;


/* cell info */
typedef struct
{
    _state state = UNOPENED; /* state of cell */
    bool ismine;             /* cell identity */
    unsigned int mines;      /* mine cout around cell */
    char repr;               /* representaton in console */
} _cell;

#endif