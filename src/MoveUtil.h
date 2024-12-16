#ifndef MOVEUTIL_H
#define MOVEUTIL_H

#include "Board.h"
#include "PieceUtil.h"
#include "SearchUtil.h"
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

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
1 bit for check flag
1 bit for checkmate flag
3 bits for promoted piece flag

Total 25 bits used out of 32.
Remaining 7 bits can be used for anything, in the future maybe if need for more flags/info is discovered.
*/

// encode move
#define encode_move(source, target, colour, piece, promoted, capture, enpassant, castling, check, checkmate, promotedType) \
    (source) |            \
    ((target) << 6) |     \
    ((colour) << 12) |    \
    ((piece) << 13) |     \
    ((promoted) << 16) |  \
    ((capture) << 17) |   \
    ((enpassant) << 18) | \
    ((castling) << 19) |  \
    ((check) << 20) |  \
    ((checkmate) << 21) |  \
    ((promotedType) << 22)\
    
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

// extract check flag
#define get_move_check(move) ((move) & 0b00000000000100000000000000000000)

// extract checkmate flag
#define get_move_checkmate(move) ((move) & 0b00000000001000000000000000000000)

// extract promoted to piece type
#define get_move_promotedType(move) (((move) & 0b00000001110000000000000000000000) >> 22)

/*

    UTIL FUNCTIONS

*/

// To get the bitboard which shows the square that are attacked by a particular side
bool isSquareAttacked(int side, Board board, int square);

// To print all the info of a move
string printMove(int move);

/*

    MOVE SCORING

*/

// Most valuable victim, least valuable attacker
// score to be accessed like MVV_LVA[attackerPiece][victimPiece]
constexpr int MVV_LVA[6][6] = {
    // Victims: Pawn, Knight, Bishop, Rook, Queen, King
    {105, 205, 305, 405, 505, 605}, // Pawn attacker
    {104, 204, 304, 404, 504, 604}, // Knight attacker
    {103, 203, 303, 403, 503, 603}, // Bishop attacker
    {102, 202, 302, 402, 502, 602}, // Rook attacker
    {101, 201, 301, 401, 501, 601}, // Queen attacker
    {100, 200, 300, 400, 500, 600}  // King attacker
};

// Killer moves
// A move that causes a beta prune is a killer move. it is likely to be good in the next search
extern int killerMoves[2][128];

// History moves
// Move that cause alpha to be upgraded. Better score if closer to root move
extern int historyMoves[2][6][64];


// Function to score a given move
inline int scoreMove(Board board, int move, int ply){
    int movePiece = get_move_piece(move);
    int moveColour = get_move_colour(move) > 0;
    int moveTarget = get_move_target(move);

    // For pv scoring
    if(scorePV){
        // check if move is part of pv
        if(pvTable[0][ply] == move){
            scorePV = 0;
            return 20000;
        }
    }

    // seperate handling of capture and quiet moves
    if(get_move_capture(move)){
        // MVV-LVA
        return MVV_LVA[movePiece][get_bit(board.pieceBitboards[1-moveColour][pawn], moveTarget) ? pawn :
                                    get_bit(board.pieceBitboards[1-moveColour][knight], moveTarget) ? knight :
                                    get_bit(board.pieceBitboards[1-moveColour][bishop], moveTarget) ? bishop :
                                    get_bit(board.pieceBitboards[1-moveColour][rook], moveTarget) ? rook :
                                    get_bit(board.pieceBitboards[1-moveColour][queen], moveTarget) ? queen :
                                    king] + 10000;
    } else {
        // First killer
        if(ply <= 63 && killerMoves[0][ply] == move)
            return 9000;
        // Second killer
        else if (ply <= 63 && killerMoves[1][ply] == move)
            return 8000;
        // History
        else
            return historyMoves[moveColour][movePiece][moveTarget];

    }
}

// Sorting algo

inline void merge(int moves[], int scores[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary arrays for moves and scores
    int* leftMoves = new int[n1];
    int* rightMoves = new int[n2];
    int* leftScores = new int[n1];
    int* rightScores = new int[n2];

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        leftMoves[i] = moves[left + i];
        leftScores[i] = scores[left + i];
    }
    for (int i = 0; i < n2; i++) {
        rightMoves[i] = moves[mid + 1 + i];
        rightScores[i] = scores[mid + 1 + i];
    }

    // Merge the temporary arrays back into moves and scores
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftScores[i] >= rightScores[j]) { // Sort by descending scores
            moves[k] = leftMoves[i];
            scores[k] = leftScores[i];
            i++;
        } else {
            moves[k] = rightMoves[j];
            scores[k] = rightScores[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements
    while (i < n1) {
        moves[k] = leftMoves[i];
        scores[k] = leftScores[i];
        i++;
        k++;
    }
    while (j < n2) {
        moves[k] = rightMoves[j];
        scores[k] = rightScores[j];
        j++;
        k++;
    }

    // Free temporary arrays
    delete[] leftMoves;
    delete[] rightMoves;
    delete[] leftScores;
    delete[] rightScores;
}

inline void mergeSort(int moves[], int scores[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(moves, scores, left, mid);
        mergeSort(moves, scores, mid + 1, right);

        // Merge the sorted halves
        merge(moves, scores, left, mid, right);
    }
}

// Function to sort a moveList
inline void sortMoves(Board& board, int ply){
    // make a array of scores
    int *scores = new int[board.moveList.count];

    // add scores
    for(int i = 0; i < board.moveList.count; i++){
        scores[i] = scoreMove(board, board.moveList.moves[i], ply);
    }
    // sort using merge sort
    mergeSort(board.moveList.moves, scores, 0, board.moveList.count - 1);

    delete[] scores;
}


#endif