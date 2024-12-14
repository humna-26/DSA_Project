#include<iostream>
#include<cstdint>
#include"PieceUtil.h"
using namespace std;

/*

UTIL FUNCTIONS

*/

void print_bitboard(uint64_t bitboard)
{
    cout << endl;

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // convert file & rank into square index
            int square = rank * 8 + file;
            
            // print ranks
            if (!file)
                cout << "  " << 8 - rank << " ";
            
            // print bit state (either 1 or 0)
            cout << " " << (get_bit(bitboard, square) > 0);
            
        }
        
        // print new line every rank
        cout << endl;
    }
    
    // print board files
    cout << "\n     a b c d e f g h\n\n";
    
    // print bitboard as unsigned decimal number
    cout << "  Bitboard: " << bitboard << endl << endl;
}

/*

ATTACK MAPS

*/

// Saad + Hassan(first 3 functions)
uint64_t genPawnAttackBB(int side, int square){
    uint64_t attacks = 0ULL;
    uint64_t pieceBB = 0ULL;
    
    set_bit(pieceBB, square);

    uint64_t file_a = file_a_bb;
    uint64_t file_h = file_a << 7;
    
    if(side == white)
    {
        if ((pieceBB >> 7) & ~file_a) attacks |= (pieceBB >> 7);
        if ((pieceBB >> 9) & ~file_h) attacks |= (pieceBB >> 9);
    }
    else if(side == black)
    {
        if ((pieceBB << 7) & ~file_h) attacks |= (pieceBB << 7);
        if ((pieceBB << 9) & ~file_a) attacks |= (pieceBB << 9);
    }
    
    return attacks;
}

uint64_t genKnightAttackBB(int square){
    uint64_t attacks = 0ULL;
    uint64_t pieceBB = 0ULL;

    uint64_t file_a = file_a_bb;
    uint64_t file_ab = file_a | (file_a_bb << 1);
    uint64_t file_h = file_a << 7;
    uint64_t file_hg = file_h | (file_a << 6);

    set_bit(pieceBB, square);

    if ((pieceBB >> 17) & ~file_h) attacks |= (pieceBB >> 17);
    if ((pieceBB >> 15) & ~file_a) attacks |= (pieceBB >> 15);
    if ((pieceBB >> 10) & ~file_hg) attacks |= (pieceBB >> 10);
    if ((pieceBB >> 6) & ~file_ab) attacks |= (pieceBB >> 6);
    if ((pieceBB << 17) & ~file_a) attacks |= (pieceBB << 17);
    if ((pieceBB << 15) & ~file_h) attacks |= (pieceBB << 15);
    if ((pieceBB << 10) & ~file_ab) attacks |= (pieceBB << 10);
    if ((pieceBB << 6) & ~file_hg) attacks |= (pieceBB << 6);

    return attacks;
}

uint64_t genKingAttackBB(int square){
    uint64_t attacks = 0ULL;
    uint64_t pieceBB = 0ULL;

    uint64_t file_a = file_a_bb;
    uint64_t file_h = file_a << 7;

    set_bit(pieceBB, square);

    if (pieceBB >> 8) attacks |= (pieceBB >> 8);
    if ((pieceBB >> 9) & ~file_h) attacks |= (pieceBB >> 9);
    if ((pieceBB >> 7) & ~file_a) attacks |= (pieceBB >> 7);
    if ((pieceBB >> 1) & ~file_h) attacks |= (pieceBB >> 1);
    if (pieceBB << 8) attacks |= (pieceBB << 8);
    if ((pieceBB << 9) & ~file_a) attacks |= (pieceBB << 9);
    if ((pieceBB << 7) & ~file_h) attacks |= (pieceBB << 7);
    if ((pieceBB << 1) & ~file_a) attacks |= (pieceBB << 1);

    return attacks;
}

uint64_t genRookAttackBB(int square){
    uint64_t attacks = 0ULL;
    
    // row and column (rank & file)
    int r, f;
    
    // init starting rank & files
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attack bitboard
    for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));
    
    // return attack bb
    return attacks;
}
uint64_t genBishopAttackBB(int square){
    uint64_t attacks = 0ULL;
    
    // row and column (rank & file)
    int r, f;
    
    // init starting rank & files
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attack bitboard
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));
    
    // return attack bb
    return attacks;
}

uint64_t genRookAttackBB(int square, uint64_t blockers){
    uint64_t attacks = 0ULL;
    
    // row and column (rank & file)
    int r, f;
    
    // init starting rank & files
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attack bitboard
    for (r = tr + 1; r <= 7; r++){
        attacks |= (1ULL << (r * 8 + tf));
        if(get_bit(blockers, r * 8 + tf)) break;
    }
    for (r = tr - 1; r >= 0; r--){
        attacks |= (1ULL << (r * 8 + tf));
        if(get_bit(blockers, r*8+tf)) break;
    }
    for (f = tf + 1; f <= 7; f++){
        attacks |= (1ULL << (tr * 8 + f));
        if(get_bit(blockers, tr*8+f)) break;
    }
    for (f = tf - 1; f >= 0; f--){
        attacks |= (1ULL << (tr * 8 + f));
        if(get_bit(blockers, tr*8+f)) break;
    }
    
    // return attack bb
    return attacks;
}
uint64_t genBishopAttackBB(int square, uint64_t blockers){
    uint64_t attacks = 0ULL;
    
    // row and column (rank & file)
    int r, f;
    
    // init starting rank & files
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attack bitboard
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++){
        attacks |= (1ULL << (r * 8 + f));
        // break when a blocker is encountered in current direction
        if (get_bit(blockers, r * 8 + f)) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++){
        attacks |= (1ULL << (r * 8 + f));
        if (get_bit(blockers, r * 8 + f)) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--){
        attacks |= (1ULL << (r * 8 + f));
        if (get_bit(blockers, r * 8 + f)) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--){
        attacks |= (1ULL << (r * 8 + f));
        if (get_bit(blockers, r * 8 + f)) break;
    }
    
    // return attack bb
    return attacks;
}

uint64_t setBlockers(int index, uint64_t attackBB){
    // init to 0
    uint64_t blockersBB = 0ULL;
    // get number of bits to loop over
    int nBits = getNBits(attackBB);
    int square;

    //loop over all the 1s
    for(int i = 0; i < nBits; i++){
        square = getLSBIndex(attackBB);
        // pop off the lsb
        pop_bit(attackBB, square);

        // if the i'th bit of the index integer is a 1, set the current square to 1
        // the current square will be the i'th lsb w.r.t the attackBB bits which are 1s.
        if(get_bit(index, i))
            set_bit(blockersBB, square);
    }
    return blockersBB;
}


// allocate memory for the maps
uint64_t pawnAttackMap[2][64];
uint64_t knightAttackMap[64];
uint64_t kingAttackMap[64];
uint64_t rookLeapMap[64];
uint64_t bishopLeapMap[64];
uint64_t rookAttackMap[64][4096];
uint64_t bishopAttackMap[64][512];

void initAttackMaps(){
    // looping over all squares as needed for all attack maps
    for(int i = 0; i < 64; i++){
        // init leaper pieces attack maps
        pawnAttackMap[white][i] = genPawnAttackBB(white, i);
        pawnAttackMap[black][i] = genPawnAttackBB(black, i);
        knightAttackMap[i] = genKnightAttackBB(i);
        kingAttackMap[i] = genKingAttackBB(i);
        
        // init rook and bishop leap maps
        rookLeapMap[i] = genRookAttackBB(i);
        bishopLeapMap[i] = genBishopAttackBB(i);

        // init rook and bishop attack maps. using the blockers stuff

        // get 2^nBits for the square. max val for bishop is 512 and for rook is 4096
        int blockerIndexBishop = 1 << getNBits(bishopLeapMap[i]);
        int blockerIndexRook = 1 << getNBits(rookLeapMap[i]);

        for(int j = 0; j < blockerIndexBishop; j++){
            // blocker BB
            uint64_t blockersBB = setBlockers(j, bishopLeapMap[i]);

            // magic index
            int magicIndex = (blockersBB * bishopMagics[i]) >> (64 - getNBits(bishopLeapMap[i]));

            // init bishop attack map using square and magic.
            bishopAttackMap[i][magicIndex] = genBishopAttackBB(i, blockersBB);

        }
        for(int j = 0; j < blockerIndexRook; j++){
            // blocker BB
            uint64_t blockersBB = setBlockers(j, rookLeapMap[i]);

            // magic index
            int magicIndex = (blockersBB * rookMagics[i]) >> (64 - getNBits(rookLeapMap[i]));

            // init rook attack map using square and magic.
            rookAttackMap[i][magicIndex] = genRookAttackBB(i, blockersBB);
        }
    }
}

/*

Zobrist Keys

*/

uint64_t pieceSquareKeys[2][6][64];
uint64_t enpassantKeys[8];
uint64_t castleRightKeys[4];
uint64_t sideToMoveKey;

// Wania
void initZobristKeys(){

}