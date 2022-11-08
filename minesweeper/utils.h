#ifndef _UTILS_H
#define _UTILS_H 1

typedef enum
{
    UNOPENED,
    OPENED,
    FLAGED
} _state;

struct _cell
{
    _state state = UNOPENED; /* state of cell */
    bool ismine;             /* cell identity */
    unsigned int mines;      /* mine cout around cell */
    char repr;               /* representaton in console */
};

#endif