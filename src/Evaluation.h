#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.h"

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
    100, 100, 100, 100, 100, 100, 100, 100, // Rank 7
    75, 70, 70, 70, 70, 70, 70, 75,    // Rank 6
    40, 40, 40, 55, 55, 40, 40, 40,    // Rank 5
    20, 20, 20, 50, 50, 20, 20, 20,    // Rank 4
    8, 8, 8, 8, 8, 8, 8, 8,     // Rank 3
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
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 20, 20, 20, 20, 20, 20, 0,
    0, 20, 40, 40, 40, 40, 20, 0,
    0, 20, 40, 60, 60, 40, 20, 0,
    0, 20, 40, 60, 60, 40, 20, 0,
    0, 20, 40, 40, 40, 40, 20, 0,
    0, 20, 20, 20, 20, 20, 20, 0,
    0, 0, 0, 0, 0, 0, 0, 0
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
constexpr int spaceBonus = 2;
// To be added for each friendly piece around the king
constexpr int kingSafetyBonus = 5;
// To be added if king can castle
constexpr int canCastleBonus = 20;


/*

    FUNCTIONS

*/

int evaluatePosition(Board board);

#endif