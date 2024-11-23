#include "Board.h"
#include "PieceUtil.h"
#include "MoveUtil.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

Board::Board() {
    // Initialize all bitboards to 0
    memset(pieceBitboards, 0, sizeof(pieceBitboards));
    memset(occupancyBitboards, 0, sizeof(occupancyBitboards));
    memset(attackingBitboards, 0, sizeof(attackingBitboards));
    
    // Initialize game state
    zobristHash = 0ULL;
    sideToMove = white;
    castleRights = 0;
    enpassantSquare = -1;
    halfMoveClocks[0] = 0;  // halfmove clock
    halfMoveClocks[1] = 1;  // fullmove number

    // init move list
    moveList = MoveList();
}

void Board::initFromFen(char* FEN) {
    // Clear the board first
    memset(pieceBitboards, 0, sizeof(pieceBitboards));
    memset(occupancyBitboards, 0, sizeof(occupancyBitboards));
    
    int square = 0;
    char* currentChar = FEN;
    
    // 1. Parse piece positions
    while (*currentChar != ' ' && square < 64) {
        if (*currentChar >= '1' && *currentChar <= '8') {
            // Skip empty squares
            square += (*currentChar - '0');
        }
        else if (*currentChar == '/') {
            // Move to next rank
            // Do nothing as square is already correctly positioned
        }
        else {
            // Place piece
            int piece, color;
            switch(*currentChar) {
                case 'P': piece = pawn; color = white; break;
                case 'N': piece = knight; color = white; break;
                case 'B': piece = bishop; color = white; break;
                case 'R': piece = rook; color = white; break;
                case 'Q': piece = queen; color = white; break;
                case 'K': piece = king; color = white; break;
                case 'p': piece = pawn; color = black; break;
                case 'n': piece = knight; color = black; break;
                case 'b': piece = bishop; color = black; break;
                case 'r': piece = rook; color = black; break;
                case 'q': piece = queen; color = black; break;
                case 'k': piece = king; color = black; break;
                default: currentChar++; continue;
            }
            set_bit(pieceBitboards[color][piece], square);
            square++;
        }
        currentChar++;
    }
    
    currentChar++;
    
    // 2. Parse side to move
    sideToMove = (*currentChar == 'w') ? white : black;
    currentChar += 2; // Skip to castling rights
    
    // 3. Parse castling rights
    castleRights = 0;
    if (*currentChar == '-') {
        currentChar += 2;
    } else {
        while (*currentChar != ' ') {
            switch(*currentChar) {
                case 'K': castleRights |= 1; break;     // White kingside
                case 'Q': castleRights |= 2; break;     // White queenside
                case 'k': castleRights |= 4; break;     // Black kingside
                case 'q': castleRights |= 8; break;     // Black queenside
            }
            currentChar++;
        }
        currentChar++; 
    }
    
    // 4. Parse en passant square
    if (*currentChar == '-') {
        enpassantSquare = -1;
        currentChar += 2;
    } else {
        int file = currentChar[0] - 'a';
        int rank = 8 - (currentChar[1] - '0');
        enpassantSquare = rank * 8 + file;
        currentChar += 3;
    }
    
    // 5. Parse halfmove clock
    halfMoveClocks[0] = atoi(currentChar);
    while (*currentChar != ' ') currentChar++;
    currentChar++;
    
    // 6. Parse fullmove number
    halfMoveClocks[1] = atoi(currentChar);
    
    // Update occupancy bitboards
    for (int piece = 0; piece < 6; piece++) {
        occupancyBitboards[white] |= pieceBitboards[white][piece];
        occupancyBitboards[black] |= pieceBitboards[black][piece];
    }
    occupancyBitboards[2] = occupancyBitboards[white] | occupancyBitboards[black];

    // Initialize the attackingBitboards using the attackedMap function
    attackingBitboards[white] = attackedMap(white, *this);
    attackingBitboards[black] = attackedMap(black, *this);
    attackingBitboards[noColour] = attackingBitboards[white] | attackingBitboards[black];
}

void Board::printBoard() const {}

// Generates all legal moves and adds to move list
void Board::generateMoves(){
    // To be filled
}

// Makes a move by updating the board accordingly
void Board::makeMove(int move){
    // To be filled
}