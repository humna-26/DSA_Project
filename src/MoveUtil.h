#ifndef MOVEUTIL_H
#define MOVEUTIL_H

#include "Board.h"
#include "PieceUtil.h"

/*

    UTIL MACROS

*/

/*
Moves are encoded in 32 bit integers.
6 bits for source square
6 bits for target square
1 bit for piece colour
3 bits for piece type
1 bit for promoted flag
1 bit for capture flag
1 bit for enpassant flag
1 bit for castling flag
3 bits for promoted piece flag

Total 23 bits used out of 32.
Remaining 9 bits can be used for anything, in the future maybe if need for more flags/info is discovered.
*/

// encode move
#define encode_move(source, target, colour, piece, promoted, capture, enpassant, castling, promotedType) \
    (source) |            \
    ((target) << 6) |     \
    ((colour) << 12) |    \
    ((piece) << 13) |     \
    ((promoted) << 16) |  \
    ((capture) << 17) |   \
    ((enpassant) << 18) | \
    ((castling) << 19) |  \
    ((promotedType) << 20)\
    
// extract source square
#define get_move_source(move) ((move) & 0b00000000000000000000000000111111)

// extract target square
#define get_move_target(move) (((move) & 0b00000000000000000000111111000000) >> 6)

// extract colour
#define get_move_colour(move) ((move) & 0b00000000000000000001000000000000)

// extract piece
#define get_move_piece(move) (((move) & 0b00000000000000001110000000000000) >> 13)

// extract promoted piece
#define get_move_promoted(move) ((move) & 0b00000000000000010000000000000000)

// extract capture flag
#define get_move_capture(move) ((move) & 0b00000000000000100000000000000000)

// extract enpassant flag
#define get_move_enpassant(move) ((move) & 0b00000000000001000000000000000000)

// extract castling flag
#define get_move_castling(move) ((move) & 0b00000000000010000000000000000000)

// extract promoted to piece type
#define get_move_promotedType(move) (((move) & 0b00000000011100000000000000000000) >> 20)

/*

    UTIL FUNCTIONS

*/

// To get the bitboard which shows the square that are attacked by a particular side
bool isSquareAttacked(int side, Board board, int square);

// To print all the info of a move
void printMove(int move);

#endif