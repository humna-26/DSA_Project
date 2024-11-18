#ifndef PIECEUTIL_H
#define PIECEUTIL_H

#include<cstdint>

/*

ENUMS

*/

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

enum { white, black};



/*

MACROS AND UTIL FUNCTIONS

*/

// sets square to 1 in a bb. eg set_bit(board, e7);
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
// gets bit at square
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
// pops bit at square
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

// function to print a bitboard in a 8x8 grid with some other useful info
// using the Board.printBoard() function instead
//void print_bitboard(uint64_t bitboard);



/*

ATTACK MAPS

*/

extern uint64_t pawnAttackMap[2][64];
extern uint64_t knightAttackMap[64];
extern uint64_t kingAttackMap[64];
extern uint64_t rookAttackMap[64];
extern uint64_t bishopAttackMap[64];
extern uint64_t queenAttackMap[64];


extern uint64_t getPawnAttackBB(int side, int square);
extern uint64_t getKnightAttackBB(int square);
extern uint64_t getKingAttackBB(int square);
extern uint64_t getRookAttackBB(int square);
extern uint64_t getBishopAttackBB(int square);
extern uint64_t getQueenAttackBB(int square);

extern void initAttackMaps();

/*

Zobrist Keys

*/

extern uint64_t pieceSquareKeys[2][6][64];
extern uint64_t enpassantKeys[8];
extern uint64_t castleRightKeys[16];
extern uint64_t sideToMoveKey;

extern void initZobristKeys();

#endif