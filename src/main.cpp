#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "SearchUtil.h"
#include "MoveUtil.h"
#include "Evaluation.h"
#include "Transposition.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
static int defaultSearchDepth = 8;

void play();
void playSelf();

int main() {
    initAttackMaps();
    initZobristKeys();

    play();
    int sizeMB = 64;
    cout << "Enter size: ";
    cin >> sizeMB;
    int nEntries = (sizeMB * 1024 * 1024) / sizeof(TTEntry);

    TranspositonTable tt = TranspositonTable(nEntries);

    srand(time(0));

    uint64_t *arr1 = new uint64_t[nEntries];
    int *arr2 = new int[nEntries];

    int wrong = 0;
    int empty = 0;

    for (int i = 0; i < nEntries; i++) {
        arr1[i] = (static_cast<uint64_t>(rand()) << 32) | rand();
        arr2[i] = rand() % 20001 - 10000;

        tt.store(TTEntry(arr1[i], arr2[i]));

        TTEntry *en = tt.search(arr1[i]);
        if(en == nullptr)
            empty++;
        else if(en->evaluation != arr2[i])
            wrong++;
    }

    cout << "Total: " << nEntries << endl;
    cout << "Wrong Eval: "<< wrong << endl;
    cout << "Null (collision): " << empty << endl << endl;

    wrong = 0;
    empty = 0;

    for(int i = 0; i < nEntries; i++){
        TTEntry *en = tt.search(arr1[i]);
        if(en == nullptr)
            empty++;
        else if(en->evaluation != arr2[i])
            wrong++;
    }

    cout << "Total: " << nEntries << endl;
    cout << "Wrong Eval: "<< wrong << endl;
    cout << "Null (collision): " << empty << endl << endl;

    cin.ignore();
    cin.get();

    cin.ignore();
    cin.get();
    return 0;
<<<<<<< HEAD
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
    char fen[] = "8/8/8/8/3k4/8/8/3KR3 w - - 0 1";
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

        pair<int, int> move = findBestMove(board, defaultSearchDepth);
        board.makeMove(move.first);
        cout << (1-board.sideToMove == white ? "White " : "Black ") << "played: " << printMove(move.first) << ", eval: " << -move.second << endl;
    }
=======
>>>>>>> transposition
}