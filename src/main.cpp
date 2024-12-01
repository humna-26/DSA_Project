#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main()
{

    Board b = Board();
    b.moveList.addMove(encode_move(e2, e4, white, pawn, false, false, false, false, false, false, noPiece));
    b.moveList.addMove(encode_move(g7, g8, white, pawn, true, false, false, false, false, false, knight));
    b.moveList.addMove(encode_move(e2, e1, black, pawn, true, false, false, false, false, false, queen));
    b.moveList.addMove(encode_move(e6, b1, white, rook, true, false, false, false, true, false, noPiece));
    b.moveList.addMove(encode_move(e2, e4, white, pawn, false, false, false, false, false, true, noPiece));

    b.moveList.printList();

    getchar();

    return 0;
}

void perft(int depth)
{
    // To be filled
}