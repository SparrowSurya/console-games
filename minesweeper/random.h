#ifndef _RANDOM_H
#define _RANDOM_H 1

#include <stdlib.h>
#include <iostream>
#include <time.h>

void seed(size_t n)
{
    srand(n);
}

/* random from 0 to end including end */
size_t random(size_t end)
{
    return rand() % (end + 1);
}

/* random from start to end including both ends */
size_t random(size_t start, size_t end)
{
    return start + (rand() % (end - start + 1));
}

void random_init()
{
    seed(time(0));
}

#endif