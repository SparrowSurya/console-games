#include <iostream>
#include <time.h>

#include "game.h"
#include "mineboard.cpp"

Mineboard Board;

/* various commands */
typedef enum {
    QUIT    = 'q', /* to quit the game */
    CMDS    = 'c', /* to get list of all commands with their uses */
    STATUS  = 'i', /* to know the status of game */
    POP     = 'x', /* to hit a tile */
    FLAG    = 'f', /* to flag a tile */
    UNFLAG  = 'u', /* to unflag a tile */
    EXPAND  = 'e', /* to expand recursively on cells who have mines around them detected */
    NEW     = 'n', /* to start new match*/
    RESIZE  = 'r'  /* to resize the mineboard dimensions */
} cmd;


void show_info() {
    std::cout << std::endl;
    std::cout << "Commands: \n\n" <<
    (char) cmd::QUIT   << " - to quit the program  \n" <<
    (char) cmd::CMDS   << " - displays all the commands  \n" <<
    (char) cmd::NEW    << " - starts new game  \n" <<
    (char) cmd::STATUS << " - show status of game  \n" <<
    (char) cmd::POP    << " rows cols - breks the tilw at row col  \n" <<
    (char) cmd::FLAG   << " rows cols - flags the tilw at row col  \n" <<
    (char) cmd::UNFLAG << " rows cols - removes flag on tile at row col  \n" <<
    (char) cmd::EXPAND << " rows cols - work on tiles whose all neighbour mines are detected (works recursively)  \n" <<
    (char) cmd::RESIZE << " rows cols mines - resizes the board (use 0 for param to use previous settings)  \n" <<
    std::endl;
}


void GameLoop() {
    srand(time(0));
    Board.Init();

    std::cout << '\n' << "--------------------Minesweeper--------------------" << "\n\n";
    Board.Generate();
    show_info();

    char cmd_in;
    short row_in, col_in, mines_in;

    /* loop */
    while (1) {
        Board.Print();
        std::cout << ">>> ";
        std::cin >> cmd_in;

        /* actions */
        switch (cmd_in) {
            case (cmd::POP): {
                std::cin >> row_in >> col_in;
                Board.Pop(row_in, col_in);
            } break;

            case (cmd::FLAG): {
                std::cin >> row_in >> col_in;
                Board.Flag(row_in, col_in);
            } break;

            case (cmd::UNFLAG): {
                std::cin >> row_in >> col_in;
                Board.Unflag(row_in, col_in);
            } break;

            case (cmd::EXPAND): {
                std::cin >> row_in >> col_in;
                Board.Expand(row_in, col_in);
            } break;

            case (cmd::NEW): {
                Board.Reset();
                Board.Generate();
                std::cout << "\n----------New-Game----------\n";
            } break;

            case (cmd::RESIZE): {
                std::cin >> row_in >> col_in >> mines_in;
                Board.Resize(row_in, col_in, mines_in);
                Board.Generate();
                std::cout << "\n----------New-Game----------\n";
            } break;

            case (cmd::CMDS): {
                show_info();
            } break;

            case (cmd::QUIT): {
                return;
            } break;

            default: {
                std::cout << "Invalid Command! \n\n";
            }
        }
    }
}

int main() {
    GameLoop();
    return 0;
}