#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.h"

/*

    EVALUTION VALUES

*/

// Material value for pieces
constexpr int pieceValues[] = {
    100, // pawn
    300, // knigth
    320, // bishop
    500, // rook
    900, // queen
    100000 // king
};

// Bonus for pawns according to how close they are to promoting.
constexpr int pawnPositionalValue[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,    // Rank 8
    100, 100, 100, 100, 100, 100, 100, 100, // Rank 7
    75, 70, 70, 70, 70, 70, 70, 75,    // Rank 6
    45, 40, 40, 35, 35, 40, 40, 45,    // Rank 5
    25, 20, 20, 30, 30, 20, 20, 25,    // Rank 4
    8, 8, 8, 8, 8, 8, 8, 8,     // Rank 3
    0, 0, 0, 0, 0, 0, 0, 0,     // Rank 2
    0, 0, 0, 0, 0, 0, 0, 0      // Rank 1
};

// Bonus for knights according to how close to center they are
constexpr int knightPositionalValue[64] = {
    0,  10,  10,  10,  10,  10,  10,  0,    // Rank 8
    10, 35, 35, 35, 35, 35, 35,  10,    // Rank 7
    10, 35, 60, 60, 60, 60, 35,  10,    // Rank 6
    10, 35, 60, 80, 80, 60, 35,  10,    // Rank 5
    10, 35, 60, 80, 80, 60, 35,  10,    // Rank 4
    10, 35, 60, 60, 60, 60, 35,  10,    // Rank 3
    10, 35, 35, 35, 35, 35, 35,  10,    // Rank 2
    0,  10,  10,  10,  10,  10,  10,  0     // Rank 1
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
    20, 20, 0, 0, 0, 0, 20, 20   // Rank 1
};


// All Values are to be added for white, and subtracted for black

// To be added for each pawn protected by another pawn
constexpr int pawnStructureBonus = 20;
// To be added if the bishop pair is present
constexpr int bishopPairBonus = 80;
// To be added for each passed pawn
constexpr int passedPawnBonus = 50;
// To be added for each isolated pawn
constexpr int isolatedPawnBonus = -30;
// To be added for each doubled pawn
constexpr int doubledPawnBonus = -8;
// To be multiplied by the number of squares attacked
constexpr int spaceBonus = 5;
// To be added for each friendly piece around the king
constexpr int kingSafetyBonus = 5;
// To be added if king can castle
constexpr int canCastleBonus = 20;


/*

    FUNCTIONS

*/

int evaluatePosition(Board board);

#endif