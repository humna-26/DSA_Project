#include "evaluation.h"
#include "Board.h"
#include "PieceUtil.h"

inline int materialValue(Board board){
    // To be 
    return 0;
}

inline int positionalValue(Board board){
    // To be filled
    return 0;
}

inline int pawnStructure(Board board){
    // To be filled
    return 0;
}

inline int passedIsolatedPawns(Board board){
    // To be filled
    return 0;
}

inline int spaceAdvantage(Board board){
    // To be filled
    return 0;
}

inline int bishopPair(Board board){
    // To be filled
    return 0;
}

int evaluatePosition(Board board){
    int eval = 0;

    eval += materialValue(board);
    eval += positionalValue(board);
    eval += pawnStructure(board);
    eval += passedIsolatedPawns(board);
    eval += spaceAdvantage(board);
    eval += bishopPair(board);

    return eval;
}