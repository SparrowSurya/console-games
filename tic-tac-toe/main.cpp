#include<iostream>

void draw(char *board)
{
    std::cout << '\n';
    std::cout << " " << board[0] << " | " << board[1] << " | " << board[2] << '\n';
    std::cout << "---+---+---" << '\n';
    std::cout << " " << board[3] << " | " << board[4] << " | " << board[5] << '\n';
    std::cout << "---+---+---" << '\n';
    std::cout << " " << board[6] << " | " << board[7] << " | " << board[8] << '\n';
    std::cout << '\n';
}

void mark(char player, char *board)
{
    int index;
    do {
        std::cout << "Player-" << player << " mark the spot: ";
        std::cin >> index;
    } while (index>9 || index<1 || board[index-1]!=' ');
    board[index-1] = player;
}

char winner(char *board)
{
    if        (board[0]==board[3] && board[3]==board[6] && board[6]!=' ') {
        return board[0];
    } else if (board[1]==board[4] && board[4]==board[7] && board[7]!=' ') {
        return board[1];
    } else if (board[2]==board[5] && board[5]==board[8] && board[8]!=' ') {
        return board[1];
    } else if (board[0]==board[1] && board[1]==board[2] && board[2]!=' ') {
        return board[0];
    } else if (board[3]==board[4] && board[4]==board[5] && board[5]!=' ') {
        return board[3];
    } else if (board[6]==board[7] && board[7]==board[8] && board[8]!=' ') {
        return board[6];
    } else if (board[0]==board[4] && board[4]==board[8] && board[8]!=' ') {
        return board[0];
    } else if (board[2]==board[4] && board[4]==board[6] && board[6]!=' ') {
        return board[2];
    } else {
        return '-';
    }
}

char over(char *board)
{
    int flag = 0;
    char res = winner(board);
    if (res=='-') {
        for (int i=0; i<9; i++) {
            if (board[i] == ' ') {
                return ' ';
            }
        }
    }
    return res;
}

int main()
{
    char board[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    char p1 = 'x', p0 = 'o', player, res;
    bool run = true;

    player = p0;
    draw(board);
    while (run) 
    {
        mark(player, board);
        draw(board);
        player = player=='x'? p0:p1;
        res = over(board);

        switch (res) {
            case 'x':
                std::cout << "Player-x Won!" << std::endl;
                run = false;
                break;
            case 'o':
                std::cout << "Player-o Won!" << std::endl;
                run = false;
                break;
            case '-':
                std::cout << "Draw" << std::endl;
                run = false;
                break;
            default:
                break;
        }
    }

    return 0;
}
