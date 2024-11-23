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

    Board board = Board();
    board.initFromFen(startpos);

    int depth;
    cout << "Enter depth for perft: ";
    cin >> depth;
    cout << endl;

    perft(&board, depth);

    cin.ignore();
    cin.get();
    return 0;
}