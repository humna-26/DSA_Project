#include "evaluation.h"
#include "Board.h"
#include "PieceUtil.h"
<<<<<<< HEAD
#include <cstdint>
#include <iostream>
using namespace std;

inline int materialValue(Board board){
    int scores[] = {0, 0};

    for(int side = white; side <= black; side++){
        for(int piece = pawn; piece <= king; piece++){
            while(board.pieceBitboards[side][piece]){
                int square = getLSBIndex(board.pieceBitboards[side][piece]);
                pop_bit(board.pieceBitboards[side][piece], square);

                scores[side] += pieceValues[piece];
            }
        }
    }
    return scores[0] - scores[1];
}

inline int positionalValue(Board board){
    int scores[] ={0, 0};
    
    for(int side = white; side <= black; side++){
        // Pawns
        while(board.pieceBitboards[side][pawn]){
            int square =  getLSBIndex(board.pieceBitboards[side][pawn]);
            pop_bit(board.pieceBitboards[side][pawn], square);
            scores[side] += pawnPositionalValue[side?(63-square):square];
        }
        // Kings
        while(board.pieceBitboards[side][king]){
            int square =  getLSBIndex(board.pieceBitboards[side][king]);
            pop_bit(board.pieceBitboards[side][king], square);
            scores[side] += kingPositionalValue[side?(63-square):square];
        }
        // Knights
        while(board.pieceBitboards[side][knight]){
            int square =  getLSBIndex(board.pieceBitboards[side][knight]);
            pop_bit(board.pieceBitboards[side][knight], square);
            scores[side] += knightPositionalValue[square];
        }
    }

    return scores[0] - scores[1];
}

inline int pawnStructure(Board board){
    uint64_t pW = board.pieceBitboards[white][pawn];
    uint64_t pB = board.pieceBitboards[black][pawn];

    int whiteScore = 0;
    int blackScore = 0;

    while(pW != 0ULL){
        int square = getLSBIndex(pW);
        pop_bit(pW, square);
        whiteScore += (pawnAttackMap[white][square] & board.pieceBitboards[white][pawn]) > 0 ? pawnStructureBonus : 0;
    }
    while(pB != 0ULL){
        int square = getLSBIndex(pB);
        pop_bit(pB, square);
        blackScore += (pawnAttackMap[black][square] & board.pieceBitboards[black][pawn]) > 0 ? pawnStructureBonus : 0;
    }
    
    return (whiteScore - blackScore);
}

inline int passedIsolatedPawns(Board board){
    Board bcpy = board;
    int scores[] = {0, 0};

    for(int side = 0; side <= black; side++){
        while(bcpy.pieceBitboards[side][pawn]){
            int square = getLSBIndex(bcpy.pieceBitboards[side][pawn]);
            pop_bit(bcpy.pieceBitboards[side][pawn], square);

            // Check for doubled
            int64_t fileMask = file_a_bb << (square % 8);
            // popping and then setting because the filemask would include the pawn we are checking for
            pop_bit(fileMask, square);
            scores[side] += (fileMask & board.pieceBitboards[side][pawn]) ? doubledPawnBonus : 0;
            set_bit(fileMask, square);

            // Check for isolated
            if((square % 8) !=7) fileMask |= fileMask << 1;
            if((square % 8) !=0) fileMask |= fileMask >> 1;
            
            // popping and then setting because the filemask would include the pawn we are checking for
            pop_bit(board.pieceBitboards[side][pawn], square);
            scores[side] += (fileMask & board.pieceBitboards[side][pawn]) ? 0 : isolatedPawnBonus;
            set_bit(board.pieceBitboards[side][pawn], square);

            // Check for passed
            uint64_t mask = rank_1_bb;
            mask >>= (((63 - square) / 8) * 8);
            mask |= mask >> 8; mask |= mask >> 8; mask |= mask >> 8; mask |= mask >> 8; mask |= mask >> 8; mask |= mask >> 8; mask |= mask >> 8;
            mask &= fileMask; // mask ready to check for white passed pawns

            mask = side ? (~mask & fileMask) : mask; // change to black mask if side is black

            scores[side] += (mask & board.pieceBitboards[1-side][pawn]) ? 0 : passedPawnBonus;
        }
    }

    return scores[0] - scores[1];
}

inline int spaceAdvantage(Board board){
    uint64_t whiteAttack = 0ULL;
    uint64_t blackAttack = 0ULL;

    while(board.pieceBitboards[white][pawn] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[white][pawn]);
        pop_bit(board.pieceBitboards[white][pawn], square);
        whiteAttack |= pawnAttackMap[white][square];
    }
    while(board.pieceBitboards[white][knight] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[white][knight]);
        pop_bit(board.pieceBitboards[white][knight], square);
        whiteAttack |= knightAttackMap[square];
    }
    while(board.pieceBitboards[white][bishop] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[white][bishop]);
        pop_bit(board.pieceBitboards[white][bishop], square);
        whiteAttack |= getBishopAttackMap(square, board.occupancyBitboards[noColour]);
    }
    while(board.pieceBitboards[white][rook] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[white][rook]);
        pop_bit(board.pieceBitboards[white][rook], square);
        whiteAttack |= getRookAttackMap(square, board.occupancyBitboards[noColour]);
    }
    while(board.pieceBitboards[white][queen] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[white][queen]);
        pop_bit(board.pieceBitboards[white][queen], square);
        whiteAttack |= getQueenAttackMap(square, board.occupancyBitboards[noColour]);
    }
    while(board.pieceBitboards[white][king] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[white][king]);
        pop_bit(board.pieceBitboards[white][king], square);
        whiteAttack |= kingAttackMap[square];
    }

    while(board.pieceBitboards[black][pawn] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[black][pawn]);
        pop_bit(board.pieceBitboards[black][pawn], square);
        blackAttack |= pawnAttackMap[black][square];
    }
    while(board.pieceBitboards[black][knight] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[black][knight]);
        pop_bit(board.pieceBitboards[black][knight], square);
        blackAttack |= knightAttackMap[square];
    }
    while(board.pieceBitboards[black][bishop] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[black][bishop]);
        pop_bit(board.pieceBitboards[black][bishop], square);
        blackAttack |= getBishopAttackMap(square, board.occupancyBitboards[noColour]);
    }
    while(board.pieceBitboards[black][rook] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[black][rook]);
        pop_bit(board.pieceBitboards[black][rook], square);
        blackAttack |= getRookAttackMap(square, board.occupancyBitboards[noColour]);
    }
    while(board.pieceBitboards[black][queen] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[black][queen]);
        pop_bit(board.pieceBitboards[black][queen], square);
        blackAttack |= getQueenAttackMap(square, board.occupancyBitboards[noColour]);
    }
    while(board.pieceBitboards[black][king] != 0ULL){
        int square = getLSBIndex(board.pieceBitboards[black][king]);
        pop_bit(board.pieceBitboards[black][king], square);
        blackAttack |= kingAttackMap[square];
    }

    whiteAttack &= ~board.occupancyBitboards[white];
    blackAttack &= ~board.occupancyBitboards[black];

    int whiteScore = getNBits(whiteAttack) * spaceBonus;
    int blackScore = getNBits(blackAttack) * spaceBonus;
    return whiteScore - blackScore;
}

inline int bishopPair(Board board){
    uint64_t bW = board.pieceBitboards[white][bishop];
    uint64_t bB = board.pieceBitboards[black][bishop];
    int bWW = 0;
    int bWB = 0;
    int bBW = 0;
    int bBB = 0;

    int score = 0;

    while(bW != 0ULL){
        int square = getLSBIndex(bW);
        pop_bit(bW, square);
        if(square/8 % 2 == 0){
            if(square%8%2 == 0){
                bWW++;
            }else{
                bWB++;
            }
        }else{
            if(square%8%2 == 0){
                bWB++;
            }else{
                bWW++;
            }
        }

        if(bWW > 0 && bWB > 0){
            score += bishopPairBonus;
            break;
        }
    }
    while(bB != 0ULL){
        int square = getLSBIndex(bB);
        pop_bit(bB, square);
        if(square/8 % 2 == 0){
            if(square%8%2 == 0){
                bBW++;
            }else{
                bBB++;
            }
        }else{
            if(square%8%2 == 0){
                bBB++;
            }else{
                bBW++;
            }
        }

        if(bBW > 0 && bBB > 0){
            score -= bishopPairBonus;
            break;
        }
    }

    return score;
}

inline int kingSafety(Board board){
    int whiteAttacks = getNBits(kingAttackMap[getLSBIndex(board.pieceBitboards[white][king])] & board.occupancyBitboards[white]);
    int blackAttacks = getNBits(kingAttackMap[getLSBIndex(board.pieceBitboards[black][king])] & board.occupancyBitboards[black]);

    int whiteScore = whiteAttacks*kingSafetyBonus;
    int blackScore = blackAttacks*kingSafetyBonus;

    whiteScore += ((board.castleRights & 3) > 0) ? canCastleBonus : 0;
    blackScore += ((board.castleRights & 12) > 0) ? canCastleBonus : 0;

    return whiteScore - blackScore;
=======

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
>>>>>>> search
}

int evaluatePosition(Board board){
    int eval = 0;

    eval += materialValue(board);
    eval += positionalValue(board);
    eval += pawnStructure(board);
    eval += passedIsolatedPawns(board);
    eval += spaceAdvantage(board);
    eval += bishopPair(board);
<<<<<<< HEAD
    eval += kingSafety(board);
=======
>>>>>>> search

    return eval;
}