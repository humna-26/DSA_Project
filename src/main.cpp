#include <iostream>
#include <cstdint>
#include "SearchUtil.h"
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
#include "Evaluation.h"
#include "Transposition.h"
#include "uci.h"
#include "UCIConstants.h"
#include "TimeManager.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
static char rep[] = "r1b1k1nr/pppp1ppp/1bn2q2/8/4P3/1NN3P1/PPP2P1P/R1BQKB1R w KQkq - 0 1";
static int defaultSearchDepth = 8;

void play();
void playSelf();
void initAll();

// REMINDER TO ADD REPETITION INDEX INCREMENTS IN UCI INPUT
// WHEN TAKING IN MOVES TO SET UP A POSITION

// e.g position startpos moves e2e4 e7e5 ....
// these moves need to be added to the game history table

int main() {
    initAll();

    //Board b = Board();
    //b.initFromFen(rep);

    //findBestMove(b, defaultSearchDepth);
    //findBestMove(b, defaultSearchDepth);

    //cin.ignore();
    //cin.get();
    // Create and start UCI loop
    UCIProtocol uci;
    uci.loop();
    return 0;
}

void initAll(){
    initAttackMaps();
    initZobristKeys();
    tt.clear();
    repetitionIndex = 0;
}

int legalMoveCount(Board board){
    int n = 0;
    Board cpy;
    board.generateMoves();
    memcpy(&cpy, &board, sizeof(Board));

    for(int i = 0; i < board.moveList.count; i++){
        if(board.makeMove(board.moveList.moves[i])){
            n++;
        }
        memcpy(&board, &cpy, sizeof(board));
    }

    return n;
}

void play(){

    Board board = Board();
    char fen[] = "4rrk1/1p4p1/6qp/p1p5/2P2B2/2Pn1K2/P6P/1R5R b - - 4 33";
    board.initFromFen(startpos);

    while(true){
        board.printBoard();

        if(legalMoveCount(board) == 0){
            if(isSquareAttacked(1-board.sideToMove, board, getLSBIndex(board.pieceBitboards[board.sideToMove][king]))){
                cout << (board.sideToMove == black ? "White " : "Black ") << "Won!" << endl;
            }
            else {
                cout << "Stalemate";
            }
            break;
        }


        if(board.sideToMove == black){
            pair<int, int> move = findBestMove(board, defaultSearchDepth);
            board.makeMove(move.first);
            cout << "Black played: " << printMove(move.first) << ", eval: " << -move.second << endl;
            continue;
        }

        board.generateMoves();
        Board cpy;
        memcpy(&cpy, &board, sizeof(Board));

        for(int i = 0; i < board.moveList.count; i++){
            if(!board.makeMove(board.moveList.moves[i])){
                memcpy(&board, &cpy, sizeof(Board));
                continue;
            }
            memcpy(&board, &cpy, sizeof(Board));
            cout << i << ": " << printMove(board.moveList.moves[i]) << endl;
        }

        int move;
        cin >> move;
        if(!board.makeMove(board.moveList.moves[move])){
            cout << "Illegal move!";
            memcpy(&board, &cpy, sizeof(Board));
        }
    }
}

void playSelf(){
    Board board = Board();
    char fen[] = "8/8/8/8/4k3/8/8/2RK4 w - - 0 1";
    board.initFromFen(fen);

    while(true){
        board.printBoard();

        if(legalMoveCount(board) == 0){
            if(isSquareAttacked(1-board.sideToMove, board, getLSBIndex(board.pieceBitboards[board.sideToMove][king]))){
                cout << (board.sideToMove == black ? "White " : "Black ") << "Won!" << endl;
            }
            else {
                cout << "Stalemate";
            }
            break;
        }

        cin.ignore();
        cin.get();

        pair<int, int> move = findBestMove(board, defaultSearchDepth);
        board.makeMove(move.first);
        cout << (1-board.sideToMove == white ? "White " : "Black ") << "played: " << printMove(move.first) << ", eval: " << -move.second << endl;
    }
}