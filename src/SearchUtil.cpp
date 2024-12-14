#include "SearchUtil.h"
#include "Board.h"
#include "MoveUtil.h"
#include <chrono>
#include <cstring>
#include <iostream>

using namespace std;

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

    // Generate Moves into Move list
    board->generateMoves();

    // Loop over all the legal moves
    for(int i = 0; i < (*board).moveList.count; i++){
        // Save the board state
        memcpy(&boardCopy, board, sizeof(*board));

        // Apply the i'th move to the board
        // Need the makeMove function for this to be done properly

        if(!(*board).makeMove((*board).moveList.moves[i]))
            continue;

        // Call function with updated board and further depth
        perft_test(board, depth - 1, count);

        // Restore the board state
        memcpy(board, &boardCopy, sizeof(*board));
    }
}

// Function to handle perft. prints number of leaf nodes and the time taken to generate
void perft(Board *board, int depth){

    int countn = 0;
    int countp = 0;
    Board copy;

    cout << "Starting Perft" << endl << endl;

    auto start = chrono::high_resolution_clock::now();


    (*board).generateMoves();

    for(int i = 0; i < board->moveList.count; i++){
        memcpy(&copy, board, sizeof(Board));

        if(!board->makeMove(board->moveList.moves[i]))
            continue;

        perft_test(board, depth - 1, &countn);


        memcpy(board, &copy, sizeof(Board));
        cout << i << ": " << printMove(board->moveList.moves[i]) << ": " << countn - countp << endl;
        countp = countn;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << endl << "Perft at depth " << depth << " : " << countn << endl;
    cout << "Time taken: " << duration.count() << " ms; " << ((double)countn / duration.count()) / 1000 << " Mn/s" << endl;
}