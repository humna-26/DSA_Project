#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
#include "Evaluation.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main()
{
    initAttackMaps();

    char fen[] = "r1bqkbnr/pppp1ppp/2n5/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq - 0 1";

    Board board = Board();
    board.initFromFen(startpos);

    cin.clear();
    cin.get();

    return 0;
}


void perft(int depth)
{
    // To be filled
}