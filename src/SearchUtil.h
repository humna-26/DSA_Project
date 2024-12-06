#ifndef SEARCHUTIL_H
#define SEARCHUTIL_H

#include "Board.h"
#include "Evaluation.h"

void genMoves(Board *board);

void perft(Board *board, int depth);
static void perft_test(Board *board, int depth, int *count);

// Functions below are untested. just made a skeleton. hopefully no bugs.
// Will be tested after both evaluation and makeMove are implemented

int findBestMove(Board board);
int negamax(Board board, int alpha, int beta, int depth, int *move);

#endif