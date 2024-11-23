#include "SearchUtil.h"
#include "Board.h"
#include "MoveUtil.h"
#include <chrono>
#include <cstring>
#include <iostream>

using namespace std;

// dummy moves. Temporary
void genMoves(Board *board){
    board->moveList.count = 0;
    for(int i = 0; i < 5; i++){
        board->moveList.moves[board->moveList.count] = 0;
        board->moveList.count++;
    }
}

void perft_test(Board *board, int depth, int *count)
{
    // Base condition
    if(depth == 0){
        // Count the leaf node and return
        (*count)++;
        return;
    }
    // Save the board state
    Board boardCopy = Board();

    // load dummy moves into the move list. Temporary
    genMoves(board);

    // Loop over all the legal moves
    for(int i = 0; i < (*board).moveList.count; i++){
        // Save the board state
        memcpy(&boardCopy, board, sizeof(*board));

        // Apply the i'th move to the board
        // Need the makeMove function for this to be done properly

        // (*board).makeMove((*board).moveList.moves[i]);

        // Call function with updated board and further depth
        perft_test(board, depth - 1, count);

        // Restore the board state
        memcpy(board, &boardCopy, sizeof(*board));
    }
}

// Function to handle perft. prints number of leaf nodes and the time taken to generate
void perft(Board *board, int depth){

    int count = 0;
    cout << "Starting Perft" << endl;

    auto start = chrono::high_resolution_clock::now();
    perft_test(board, depth, &count);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << "Perft at depth " << depth << " : " << count << endl;
    cout << "Time taken: " << duration.count() << " ms" << endl;
}