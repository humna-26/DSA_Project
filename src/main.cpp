#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main() {

    initAttackMaps();

    Board board = Board();

    char fen[] = "2b5/p2NBp1p/1bp1nPPr/3P4/2pRnr1P/1k1B1Ppp/1P1P1pQP/Rq1N3K b - - 0 1";

    board.initFromFen(fen);

    print_bitboard(board.attackingBitboards[white]);
    print_bitboard(board.attackingBitboards[black]);
    print_bitboard(board.attackingBitboards[noColour]);

    getchar();
    
    return 0;
}

void perft(int depth){
    // To be filled
}