#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main()
{

    int move = encode_move(e2, e4, white, pawn, 0, 0, 0, 1, 0, 1, noPiece);
    
    cout << get_move_source(move) << endl;
    cout << get_move_target(move) << endl;

    printMove(move);

    getchar();

    return 0;
}

void perft(int depth)
{
    // To be filled
}