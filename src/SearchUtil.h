#ifndef SEARCHUTIL_H
#define SEARCHUTIL_H

#include "Board.h"

void genMoves(Board *board);

void perft(Board *board, int depth);
static void perft_test(Board *board, int depth, int *count);

#endif