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


// All Values are to be added for white, and subtracted for black

// To be added for each pawn protected by another pawn
constexpr int pawnStructureBonus = 20;
// To be added if the bishop pair is present
constexpr int bishopPairBonus = 80;
// To be added for each passed pawn
constexpr int passedPawnBonus = 50;
// To be added for each isolated pawn
constexpr int isolatedPawnBonus = -40;
// To be multiplied by the number of squares attacked
constexpr int spaceBonus = 5;


/*

    FUNCTIONS

*/

int evaluatePosition(Board board);

#endif