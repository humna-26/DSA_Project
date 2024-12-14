#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
<<<<<<< HEAD
#include "Evaluation.h"
=======
#include "SearchUtil.h"

>>>>>>> perft
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

<<<<<<< HEAD
int main()
{
    initAttackMaps();

    char fen[] = "r1bqkbnr/pppp1ppp/2n5/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq - 0 1";

    Board board = Board();
    board.initFromFen(startpos);
    board.printBoard();
    
    for(int i = 0; i < 20; i++){
        board.generateMoves();
        printMove(board.moveList.moves[0]);
        board.makeMove(board.moveList.moves[0]);
        board.printBoard();
    }


    cin.clear();
    cin.get();

    return 0;
}


void perft(int depth)
{
    // To be filled
=======
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
>>>>>>> perft
}