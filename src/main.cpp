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
    board.initFromFen(startpos);

    /*
    
    int *count = 0;
    int depth = 5;
    perft(&board, depth, count);
    cout << "perft at depth " << depth << " : " << count << endl;
    
    */

    cout << sizeof(board);

    getchar();
    
    return 0;
}

void perft(Board *board, int depth, int *count){
    // Base condition
    if(depth == 0){
        // Count the leaf node and return
        (*count)++;
        return;
    }
    // Save the board state
    Board boardCopy = Board();

    // Loop over all the legal moves
    for(int i = 0; i < (*board).moveList.count; i++){
        // Save the board state
        memcpy(&boardCopy, board, sizeof(board));

        // Apply the i'th move to the board
        // Need the makeMove function for this to be done properly

        // (*board).makeMove((*board).moveList.moves[i]);

        // Call function with updated board and further depth
        perft(board, depth - 1, count);

        // Restore the board state
        memcpy(board, &boardCopy, sizeof(board));
    }
}