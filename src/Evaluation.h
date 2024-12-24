#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.h"
#include <cmath>

/*

    EVALUTION VALUES

*/

// Material value for pieces
constexpr int pieceValues[] = {
    100, // pawn
    300, // knight
    320, // bishop
    500, // rook
    1000, // queen
    100000 // king
};

// Bonus for pawns according to how close they are to promoting.
constexpr int pawnPositionalValue[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,    // Rank 8
    90, 90, 90, 90, 90, 90, 90, 90, // Rank 7
    50, 50, 50, 50, 50, 50, 50, 50,    // Rank 6
    30, 30, 30, 70, 70, 30, 30, 30,    // Rank 5
    10, 10, 10, 70, 70, 10, 10, 10,    // Rank 4
    5, 5, 5, 5, 5, 5, 5, 5,     // Rank 3
    0, 0, 0, 0, 0, 0, 0, 0,     // Rank 2
    0, 0, 0, 0, 0, 0, 0, 0      // Rank 1
};

// Bonus for knights according to how close to center they are
constexpr int knightPositionalValue[64] = {
    -15, -15, -15, -15, -15, -15, -15, -15,
    -15, 0, 0, 0, 0, 0, 0, -15,
    -15, 0, 0, 0, 0, 0, 0, -15,
    -15, 0, 0, 40, 40, 0, 0, -15,
    -15, 0, 30, 40, 40, 30, 0, -15,
    -15, 0, 25, 15, 15, 25, 0, -15,
    -15, 0, 0, 10, 10, 0, 0, -15,
    -15, -30, -10, -10, -10, -10, -30, -15
};

// Bonus for king according to initial safety
constexpr int kingPositionalValue[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,  // Rank 8
    0, 0, 0, 0, 0, 0, 0, 0,  // Rank 7
    0, 0, 0, 0, 0, 0, 0, 0,  // Rank 6
    0, 0, 0, 0, 0, 0, 0, 0,  // Rank 5
    0, 0, 0, 0, 0, 0, 0, 0,  // Rank 4
    0, 0, 0, 0, 0, 0, 0, 0,  // Rank 3
    0, 0, 0, 0, 0, 0, 0, 0,  // Rank 2
    100, 100, 100, 0, -10, 0, 100, 100   // Rank 1
};

constexpr int kingEndGameValue[64] = {
    -200, -100, -100, -100, -100, -100, -100, -200,
    -100, 20, 20, 20, 20, 20, 20, -100,
    -100, 20, 40, 40, 40, 40, 20, -100,
    -100, 20, 40, 60, 60, 40, 20, -100,
    -100, 20, 40, 60, 60, 40, 20, -100,
    -100, 20, 40, 40, 40, 40, 20, -100,
    -100, 20, 20, 20, 20, 20, 20, -100,
    -200, -100, -100, -100, -100, -100, -100, -200
};

constexpr int rookPositionalValue[64] = {
    20, 20, 20, 20, 20, 20, 20, 20,
    25, 25, 25, 25, 25, 25, 25, 25,
    0, 0, 0, 20, 20, 0, 0, 0,
    0, 0, 0, 20, 20, 0, 0, 0,
    0, 0, 0, 20, 20, 0, 0, 0,
    0, 0, 0, 20, 20, 0, 0, 0,
    0, 0, 0, 20, 20, 0, 0, 0,
    -30, 0, 0, 20, 20, 0, 0, -30
};

constexpr int bishopPositionalValue[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 10, 10, 0, 0, 0,
    0, 0, 12, 15, 15, 12, 0, 0,
    0, 0, 10, 20, 20, 10, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 30, 0, 0, 0, 0, 30, 0,
    0, 0, -30, 0, 0, -30, 0, 0
};


// All Values are to be added for white, and subtracted for black

// To be added for each pawn protected by another pawn
constexpr int pawnStructureBonus = 10;
// To be added if the bishop pair is present
constexpr int bishopPairBonus = 80;
// To be added for each passed pawn
constexpr int passedPawnBonus = 50;
// To be added for each isolated pawn
constexpr int isolatedPawnBonus = -30;
// To be added for each doubled pawn
constexpr int doubledPawnBonus = -8;
// To be multiplied by the number of squares attacked
constexpr int spaceBonus = 3;
// To be added for each friendly piece around the king
constexpr int kingSafetyBonus = 15;
// To be added if king can castle
constexpr int canCastleBonus = 20;
// To be multiplied by the distance bw kings
constexpr int kingDistBonus = 40;


// number of bishops for each side, and colour of square
extern int bWW;
extern int bWB;
extern int bBW;
extern int bBB;


/*

    FUNCTIONS

*/

int evaluatePosition(Board board);

inline int getDist(int square1, int square2){
    // rows
    int r1 = square1 / 8;
    int r2 = square2 / 8;

    // columns
    int c1 = square1 % 8;
    int c2 = square2 % 8;

    // distance formula
    return (int)sqrt((abs(r1 - r2))*(abs(r1 - r2)) + (abs(c1 - c2))*(abs(c1 - c2)));
}

#endif