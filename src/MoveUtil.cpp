#include "MoveUtil.h"
#include "PieceUtil.h"
#include "Board.h"

/*

    UTIL FUNCTIONS

*/

// Function to tell if given square is attacked by the given side. wont work with noColour.
// Will return true if square has a piece of a particular side, and the same side is given.
// e.g if e4 has a white queen, and we are asking if white is attacking e4, if any other white piece is indeed attacking e4, function will return true.
// This does not mean the piece can actually move there
// This shouldn't be a problem as there is no point in asking if white is attacking its own queen.
bool isSquareAttacked(int side, Board board, int square){
    // Basic approach is to AND the bitboard of all pieces with their attack maps at the given square one by one.
    // e.g for a knight. for the given sqaure, the knight can move to some particular squares. if there is a knight on one of those squares of the given side, then the given square is attacked
    // same method for all pieces

    // Check for pawn attack
    if(board.pieceBitboards[side][pawn] & pawnAttackMap[side == white ? black : white][square]) return 1;

    // Check for knight attack
    if(board.pieceBitboards[side][knight] & knightAttackMap[square]) return 1;

    // Check for king attack
    if(board.pieceBitboards[side][king] & kingAttackMap[square]) return 1;

    // Check for bishop attack
    if(board.pieceBitboards[side][bishop] & getBishopAttackMap(square, board.occupancyBitboards[noColour])) return 1;

    // Check for rook attack
    if(board.pieceBitboards[side][rook] & getRookAttackMap(square, board.occupancyBitboards[noColour])) return 1;

    // Check for queen attack
    if(board.pieceBitboards[side][queen] & getQueenAttackMap(square, board.occupancyBitboards[noColour])) return 1;

    // False if nothing is attacking the square
    return false;
}

void printMove(int move){
    // To be filled
}