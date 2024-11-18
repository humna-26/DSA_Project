#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
using namespace std;

int main() {
    Board board;
    
    // Initialize with starting position
    char startpos[] = "r1bqk1nr/pppp1ppp/2n5/2b5/4P3/1N6/PPP2PPP/RNBQKB1R w KQkq - 0 1";
    board.initFromFen(startpos);

    getchar();
    
    return 0;
}