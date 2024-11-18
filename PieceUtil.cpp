#include<iostream>
#include<cstdint>
#include"PieceUtil.h"
using namespace std;

// using the Board.printBoard() function instead
/*
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
*/

/*

ATTACK MAPS

*/

uint64_t getPawnAttackBB(int side, int square){ return 0ULL;}
uint64_t getKnightAttackBB(int square){ return 0ULL;}
uint64_t getKingAttackBB(int square){ return 0ULL;}
uint64_t getRookAttackBB(int square){ return 0ULL;}
uint64_t getBishopAttackBB(int square){ return 0ULL;}
uint64_t getQueenAttackBB(int square){ return 0ULL;}

void initAttackMaps(){}

/*

Zobrist Keys

*/

void initZobristKeys(){}