#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main()
{

    char fen[] = "r1bqkbnr/p1pp2pp/2n2p2/1B2p3/3PP2P/5N2/PPP3P1/RNBQK2R w KQkq - 0 1";
    uint64_t bb = 0ULL;
    Board board = Board();
    board.initFromFen(fen);
    board.printBoard();

    getchar();

    return 0;
}

void perft(int depth)
{
    // To be filled
}