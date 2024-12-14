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

    cout << "Perft at depth " << depth << " : " << countn << endl;
    cout << "Time taken: " << duration.count() << " ms; " << ((double)countn / duration.count()) / 1000 << " Mn/s" << endl;
}

// Negamax search with alpha-beta pruning
// depth is in plies
int negamax(Board board, int alpha, int beta, int depth, int ply, int *move){
    // Base condition
    if(depth == 0){
        return evaluatePosition(board);
    }

    board.generateMoves();

    // 2nd Base condition, to end the search in case there are no more moves to play
    if(board.moveList.count == 0){
        // End of current branch
        // Code here to check for stalemate or checkmate and return score accordingly
        // Haven't decided yet if mates should be checked for in evaluation or outside of it
        return evaluatePosition(board);
    }

    // declare variable to store board copy in
    Board boardCopy;

    int currAlpha = alpha;
    int bestMove;

    // Loop over all moves
    for(int i = 0; i < board.moveList.count; i++){

        // Save current board config
        memcpy(&boardCopy, &board, sizeof(board));

        // Code here to make the ith move
        // Next line should be un-commented when makeMove is implemented
        // board.makeMove(board.moveList.moves[i]);

        // Increment and decrement ply to keep track of depth
        // update current node's score with -negamax of child node
        ply++;
        int score = -negamax(board, -beta, -alpha, depth - 1, move);
        ply--;

        // Load back the current config
        memcpy(&board, &boardCopy, sizeof(board));

        // prune for beta fail
        if(score >= beta){
            return beta;
        }

        // Found a better move
        if(score > alpha){
            alpha = score;

            if(ply == 0){
                bestMove = board.moveList.moves[i];
            }
        }
    }

    // if alpha has changed, better move was found.
    if(currAlpha != alpha){
        *move = bestMove;
    }

    // return the eval of the best move that is reachable
    return alpha;
}

// General function to find best move, using various algorithms and techniques
// Just negamax with depth 5 for now
// Won't be using just negamax with fixed depth in future
// Also the way this function is written is bad, as the eval (return value from negamax) is lost.
// Maybe make this function std::pair<> in future
int findBestMove(Board board){
    int move = 0;

    // depth hard coded to 5 for now. later will be controlled using iterative deepening
    negamax(board, INT_MAX, INT_MIN, 5, &move);

    return move;
}