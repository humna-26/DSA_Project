#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
#include "SearchUtil.h"

using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main() {
    initAttackMaps();

    char fen[] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
    Board board = Board();
    board.initFromFen(fen);

    board.printBoard();

    board.generateMoves();

    int depth;
    cout << "Enter depth for perft: ";
    cin >> depth;
    cout << endl;

    perft(&board, depth);

    cin.ignore();
    cin.get();
    return 0;
}