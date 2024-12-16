#ifndef SEARCHUTIL_H
#define SEARCHUTIL_H

#include "Board.h"
#include "Evaluation.h"
#include "Transposition.h"
#include <utility>

using namespace std;

void genMoves(Board *board);

// Functions for perft

void perft(Board *board, int depth);
static void perft_test(Board *board, int depth, int *count);

// Functions to search for best move

std::pair<int, int> findBestMove(Board board, int depth);
int negamax(Board board, int alpha, int beta, int depth, int ply, int *move, int *nodes);
int quiescenseSearch(Board board, int alpha, int beta, int ply, int *nodes);

// PV arrays and flags
extern int pvLength[128];
extern int pvTable[128][128];
extern int followPV;
extern int scorePV;

// late move reduction constants
constexpr int fullDepthMoves = 4;
constexpr int reductionLimit = 3;

// null move reduction minimum depth requirement
constexpr int nmMinDepth = 3;

// Aspiration window
constexpr int aspirationWindow = 50;

// scoring constants
constexpr int infinity = 2147483640;
constexpr int mateScore = 200000000;
constexpr int mateLower = 190000000;

// For 3-fold repetitions
// max number of moves in a game is 8849.5
// that makes 17699 total possible positions in a game. (not necessarily unique)
// set to 17700 because it never will get overflowed
extern uint64_t gameHistoryTable[17700];
extern int repetitionIndex;

inline bool repetitionDetection(uint64_t hash){
    for(int i = 0; i < repetitionIndex; i++){
        if(hash == gameHistoryTable[i])
            return true;
    }
    return false;
}

#endif