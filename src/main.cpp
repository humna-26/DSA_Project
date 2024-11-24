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

    char fen[] = "r1bqk1nr/pppp1p2/2n1Q1p1/2b1p1Bp/3PP3/5NP1/PPP1BP1P/RN2K2R w KQkq - 0 1";

    board.initFromFen(fen);

    uint64_t attackedMapW = 0ULL;
    uint64_t attackedMapB = 0ULL;

    for(int i = 0; i < 64; i++){
        if(isSquareAttacked(white, board, i)) set_bit(attackedMapW, i);
        if(isSquareAttacked(black, board, i)) set_bit(attackedMapB, i);
    }

    print_bitboard(attackedMapW);
    print_bitboard(attackedMapB);

    getchar();
    
    return 0;
}

void perft(int depth){
    // To be filled
}