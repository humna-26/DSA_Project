#include "MoveUtil.h"
#include "PieceUtil.h"
#include "Board.h"

/*

    UTIL FUNCTIONS

*/

uint64_t attackedMap(int side, Board board){
    // init bitboard to 0
    uint64_t result = 0ULL;

    // OR the bitboard by all the attack maps of given colour
    for(int square = 0; square < 64; square++){
        for(int piece = 0; piece < 6; piece++){
            if(get_bit(board.pieceBitboards[side][piece], square)){
                switch(piece){
                    case(pawn): result |= pawnAttackMap[side][square]; continue;
                    case(knight): result |= knightAttackMap[square]; continue;
                    case(king): result |= kingAttackMap[square]; continue;
                    case(bishop): result |= getBishopAttackMap(square, board.occupancyBitboards[noColour]); continue;
                    case(rook): result |= getRookAttackMap(square, board.occupancyBitboards[noColour]); continue;
                    case(queen): result |= getQueenAttackMap(square, board.occupancyBitboards[noColour]); continue;
                }
            }
        }
    }

    result &= ~board.occupancyBitboards[side];

    return result;
}

void printMove(int move){
    // To be filled
}