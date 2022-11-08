#ifndef MINEBOARD_H
#define MINEBOARD_H 1

#include <iostream>
#include "base\_mineboard.h"
#include "base\random.h"
#include "settings.h"

/* game namespace */
namespace Minesweeper
{

    /* mineboard object for handling board */
    class MineBoard: private Minesweeper::_mineboard;

} // namespace Minesweeper

#endif