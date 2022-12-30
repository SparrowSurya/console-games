#include <iostream>
#include <time.h>

#include "game.h"
#include "mineboard.cpp"


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
    (char) cmd::POP    << " rows cols - breks the tile at row col  \n" <<
    (char) cmd::FLAG   << " rows cols - flags the tile at row col  \n" <<
    (char) cmd::UNFLAG << " rows cols - removes flag on tile at row col  \n" <<
    (char) cmd::EXPAND << " rows cols - work on tiles whose all neighbour mines are detected (works recursively)  \n" <<
    (char) cmd::RESIZE << " rows cols mines - resizes the board (use 0 for param to use previous settings)  \n" <<
    std::endl;
}


void GameLoop() {
    srand(time(0));
    Mineboard Board;
    Board.Init();

    std::cout << '\n' << "--------------------Minesweeper--------------------" << "\n\n";
    show_info();

    char cmd_in = 0;
    short row_in, col_in, mines_in;

    /* loop */
    while (1) {
        if (cmd_in != cmd::CMDS) {
            Board.Print();
        }
        std::cout << (char)175 << ' ';
        std::cin >> cmd_in;

        /* actions */
        switch (cmd_in) {
            case (cmd::POP): {
                std::cin >> row_in >> col_in;
                if (!Board.Pop(row_in, col_in)) {
                    std::cout << "Invalid tile to pop \n";
                }
            } break;

            case (cmd::FLAG): {
                std::cin >> row_in >> col_in;
                if (!Board.Flag(row_in, col_in)) {
                    std::cout << "Invalid tile to flag \n";
                }
            } break;

            case (cmd::UNFLAG): {
                std::cin >> row_in >> col_in;
                if (!Board.Unflag(row_in, col_in)) {
                    std::cout << "Invalid tile to unflag \n";
                }
            } break;

            case (cmd::EXPAND): {
                std::cin >> row_in >> col_in;
                if (!Board.Expand(row_in, col_in)) {
                    std::cout << "Invalid tile to expand \n";
                }
            } break;

            case (cmd::STATUS): {
                std::cout <<
                    "Total Tiles: "   << Board.GetRows()*Board.GetCols() << '\n' <<
                    "Total Mines: "   << Board.GetMines()                << '\n' <<
                    "Tiles Broken: "  << Board.GetBrokenTiles()          << '\n' <<
                    "Tiles Flagged: " << Board.GetFlaggedTiles()         << '\n' <<
                    '\n';
            } break;

            case (cmd::NEW): {
                Board.Reset();
                std::cout << "\n----------New-Game----------\n";
            } break;

            case (cmd::RESIZE): {
                std::cin >> row_in >> col_in >> mines_in;
                if (!Board.Resize(row_in, col_in, mines_in)) {
                    std::cout << "Invalid Constraints to resize board! \n" << 
                            "Rows defined range: [" << MINROW << ',' << MAXROW << "] \n" <<
                            "Cows defined range: [" << MINCOL << ',' << MAXCOL << "] \n";
                } else {
                    std::cout << "\n----------New-Game----------\n";
                }
            } break;

            case (cmd::CMDS): {
                show_info();
            } break;

            case (cmd::QUIT): {
                return;
            } break;

            default: {
                std::cout << "Invalid Command! \nUse \'" << (char) cmd::CMDS << "\' to get commands! \n\n";
            }
        }

        /* game won|lost|continue decision */
        if (Board.Explosion()) {
            Board.Print(true);
            std::cout << "You Lost! \n";
        } else if (Board.Demined()) {
            std::cout << "You Won! \n";
        } else {
            continue;
        }
        std::cout << "Quit the game? [Y/n]";
        std::cin >> cmd_in;

        if (cmd_in == 'Y') {
            return;
        } else {
            Board.Reset();
            std::cout << "\n----------New-Game----------\n";
        }
    }
}

int main() {
    GameLoop();
    return 0;
}