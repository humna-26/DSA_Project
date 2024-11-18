#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
using namespace std;

int main() {
    Board board;
    
    // Initialize with starting position
    char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board.initFromFen(startpos);
    
    board.printBoard();
    
    return 0;
}