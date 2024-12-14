#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include "MoveList.h"

class Board {
    public:
        Board();
        uint64_t pieceBitboards[2][6];
        uint64_t occupancyBitboards[3];

        uint64_t zobristHash;

        int sideToMove;
        int castleRights;
        int enpassantSquare;
        int halfMoveClocks[2];

        void initFromFen(char* FEN);
        void printBoard() const;

        MoveList moveList;

        void generateMoves();
        bool makeMove(int move);
};

#endif