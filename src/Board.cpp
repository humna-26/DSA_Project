#include "Board.h"
#include "PieceUtil.h"
#include "MoveUtil.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

Board::Board()
{
    // Initialize all bitboards to 0
    memset(pieceBitboards, 0, sizeof(pieceBitboards));
    memset(occupancyBitboards, 0, sizeof(occupancyBitboards));

    // Initialize game state
    zobristHash = 0ULL;
    sideToMove = white;
    castleRights = 0;
    enpassantSquare = -1;
    halfMoveClocks[0] = 0; // halfmove clock
    halfMoveClocks[1] = 1; // fullmove number

    // init move list
    moveList = MoveList();
}

void Board::initFromFen(char *FEN)
{
    // Clear the board first
    memset(pieceBitboards, 0, sizeof(pieceBitboards));
    memset(occupancyBitboards, 0, sizeof(occupancyBitboards));

    int square = 0;
    char *currentChar = FEN;

    // 1. Parse piece positions
    while (*currentChar != ' ' && square < 64)
    {
        if (*currentChar >= '1' && *currentChar <= '8')
        {
            // Skip empty squares
            square += (*currentChar - '0');
        }
        else if (*currentChar == '/')
        {
            // Move to next rank
            // Do nothing as square is already correctly positioned
        }
        else
        {
            // Place piece
            int piece, color;
            switch (*currentChar)
            {
            case 'P':
                piece = pawn;
                color = white;
                break;
            case 'N':
                piece = knight;
                color = white;
                break;
            case 'B':
                piece = bishop;
                color = white;
                break;
            case 'R':
                piece = rook;
                color = white;
                break;
            case 'Q':
                piece = queen;
                color = white;
                break;
            case 'K':
                piece = king;
                color = white;
                break;
            case 'p':
                piece = pawn;
                color = black;
                break;
            case 'n':
                piece = knight;
                color = black;
                break;
            case 'b':
                piece = bishop;
                color = black;
                break;
            case 'r':
                piece = rook;
                color = black;
                break;
            case 'q':
                piece = queen;
                color = black;
                break;
            case 'k':
                piece = king;
                color = black;
                break;
            default:
                currentChar++;
                continue;
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
    if (*currentChar == '-')
    {
        currentChar += 2;
    }
    else
    {
        while (*currentChar != ' ')
        {
            switch (*currentChar)
            {
            case 'K':
                castleRights |= 1;
                break; // White kingside
            case 'Q':
                castleRights |= 2;
                break; // White queenside
            case 'k':
                castleRights |= 4;
                break; // Black kingside
            case 'q':
                castleRights |= 8;
                break; // Black queenside
            }
            currentChar++;
        }
        currentChar++;
    }

    // 4. Parse en passant square
    if (*currentChar == '-')
    {
        enpassantSquare = -1;
        currentChar += 2;
    }
    else
    {
        int file = currentChar[0] - 'a';
        int rank = 8 - (currentChar[1] - '0');
        enpassantSquare = rank * 8 + file;
        currentChar += 3;
    }

    // 5. Parse halfmove clock
    halfMoveClocks[0] = atoi(currentChar);
    while (*currentChar != ' ')
        currentChar++;
    currentChar++;

    // 6. Parse fullmove number
    halfMoveClocks[1] = atoi(currentChar);

    // Update occupancy bitboards
    for (int piece = 0; piece < 6; piece++)
    {
        occupancyBitboards[white] |= pieceBitboards[white][piece];
        occupancyBitboards[black] |= pieceBitboards[black][piece];
    }
    occupancyBitboards[2] = occupancyBitboards[white] | occupancyBitboards[black];
}

void Board::printBoard() const
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

            if (!get_bit(occupancyBitboards[2], square))
            {
                cout << " -";
            }
            else
            {
                int colour = get_bit(occupancyBitboards[1], square) ? black : white;

                if (get_bit(pieceBitboards[colour][pawn], square))
                    cout << " " << (colour ? 'p' : 'P');
                else if (get_bit(pieceBitboards[colour][knight], square))
                    cout << " " << (colour ? 'n' : 'N');
                else if (get_bit(pieceBitboards[colour][bishop], square))
                    cout << " " << (colour ? 'b' : 'B');
                else if (get_bit(pieceBitboards[colour][rook], square))
                    cout << " " << (colour ? 'r' : 'R');
                else if (get_bit(pieceBitboards[colour][queen], square))
                    cout << " " << (colour ? 'q' : 'Q');
                else if (get_bit(pieceBitboards[colour][king], square))
                    cout << " " << (colour ? 'k' : 'K');
            }
        }

        // print new line every rank
        cout << endl;
    }

    // print board files
    cout << "\n     a b c d e f g h" << endl << endl;
    // util info
    cout << "    Turn: " << (sideToMove ? "black" : "white") << endl;
    char castling[5] = "";
    if (get_bit(castleRights, 0))
        strcat(castling, "K");
    if (get_bit(castleRights, 1))
        strcat(castling, "Q");
    if (get_bit(castleRights, 2))
        strcat(castling, "k");
    if (get_bit(castleRights, 3))
        strcat(castling, "q");
    cout << "    Castling: " << castling << endl;
    cout << "    Enpassant: ";
    if (enpassantSquare == -1)
        cout << "none" << endl;
    else
        cout << (char)(97 + (enpassantSquare % 8)) << 8 - enpassantSquare / 8 << endl;
    cout << "    Full-moves: " << halfMoveClocks[0] << endl;
    cout << "    Half-moves: " << halfMoveClocks[1] << endl
         << endl;
}

// helper functions to be used in generateMoves
void Board::generatePawnMoves(int square) {
    uint64_t attacks = pawnAttackMap[sideToMove][square];
    uint64_t moves = 0ULL;
    int rank = square / 8;
    int startRank = (sideToMove == white) ? 6 : 1;
    int direction = (sideToMove == white) ? -8 : 8;
    
    // Regular pawn moves (one square forward)
    int targetSquare = square + direction;
    if (targetSquare >= 0 && targetSquare < 64 && !get_bit(occupancyBitboards[2], targetSquare)) {
        moves |= (1ULL << targetSquare);
        
        // Double pawn moves (two squares forward from starting position)
        if (rank == startRank) {
            targetSquare = square + 2 * direction;
            if (!get_bit(occupancyBitboards[2], targetSquare)) {
                moves |= (1ULL << targetSquare);
            }
        }
    }

    // Handle captures
    while (attacks) {
        targetSquare = getLSBIndex(attacks);
        if (get_bit(occupancyBitboards[1 - sideToMove], targetSquare)) {
            // Check for promotion
            if ((sideToMove == white && targetSquare < 8) || 
                (sideToMove == black && targetSquare >= 56)) {
                // Add promotions
                moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, true, false, false, false, false, queen));
                moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, true, false, false, false, false, rook));
                moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, true, false, false, false, false, bishop));
                moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, true, false, false, false, false, knight));
            } else {
                moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, false, true, false, false, false, false, noPiece));
            }
        }
        // Handle en passant
        else if (targetSquare == enpassantSquare) {
            moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, false, true, true, false, false, false, noPiece));
        }
        pop_bit(attacks, targetSquare);
    }

    // Handle quiet pawn moves and promotions
    while (moves) {
        targetSquare = getLSBIndex(moves);
        if ((sideToMove == white && targetSquare < 8) || 
            (sideToMove == black && targetSquare >= 56)) {
            // Add promotions
            moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, false, false, false, false, false, queen));
            moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, false, false, false, false, false, rook));
            moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, false, false, false, false, false, bishop));
            moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, true, false, false, false, false, false, knight));
        } else {
            moveList.addMove(encode_move(square, targetSquare, sideToMove, pawn, false, false, false, false, false, false, noPiece));
        }
        pop_bit(moves, targetSquare);
    }
}

void Board::generateKnightMoves(int square) {
    uint64_t attacks = knightAttackMap[square];
    attacks &= ~occupancyBitboards[sideToMove];  // Remove friendly pieces

    while (attacks) {
        int targetSquare = getLSBIndex(attacks);
        bool isCapture = get_bit(occupancyBitboards[1 - sideToMove], targetSquare);
        
        moveList.addMove(encode_move(square, targetSquare, sideToMove, knight, false, isCapture, false, false, false, false, noPiece));
        pop_bit(attacks, targetSquare);
    }
}

void Board::generateBishopMoves(int square) {
    uint64_t attacks = getBishopAttackMap(square, occupancyBitboards[2]);
    attacks &= ~occupancyBitboards[sideToMove];

    while (attacks) {
        int targetSquare = getLSBIndex(attacks);
        bool isCapture = get_bit(occupancyBitboards[1 - sideToMove], targetSquare);
        
        moveList.addMove(encode_move(square, targetSquare, sideToMove, bishop, false, isCapture, false, false, false, false, noPiece));
        pop_bit(attacks, targetSquare);
    }
}

void Board::generateRookMoves(int square) {
    uint64_t attacks = getRookAttackMap(square, occupancyBitboards[2]);
    attacks &= ~occupancyBitboards[sideToMove];

    while (attacks) {
        int targetSquare = getLSBIndex(attacks);
        bool isCapture = get_bit(occupancyBitboards[1 - sideToMove], targetSquare);
        
        moveList.addMove(encode_move(square, targetSquare, sideToMove, rook, false, isCapture, false, false, false, false, noPiece));
        pop_bit(attacks, targetSquare);
    }
}

void Board::generateQueenMoves(int square) {
    uint64_t attacks = getQueenAttackMap(square, occupancyBitboards[2]);
    attacks &= ~occupancyBitboards[sideToMove];

    while (attacks) {
        int targetSquare = getLSBIndex(attacks);
        bool isCapture = get_bit(occupancyBitboards[1 - sideToMove], targetSquare);
        
        moveList.addMove(encode_move(square, targetSquare, sideToMove, queen, false, isCapture, false, false, false, false, noPiece));
        pop_bit(attacks, targetSquare);
    }
}

void Board::generateKingMoves(int square) {
    uint64_t attacks = kingAttackMap[square];
    attacks &= ~occupancyBitboards[sideToMove];

    // Normal moves
    while (attacks) {
        int targetSquare = getLSBIndex(attacks);
        bool isCapture = get_bit(occupancyBitboards[1 - sideToMove], targetSquare);
        
        moveList.addMove(encode_move(square, targetSquare, sideToMove, king, false, isCapture, false, false, false, false, noPiece));
        pop_bit(attacks, targetSquare);
    }

    // Castling moves
    if (sideToMove == white) {
        if (get_bit(castleRights, 0) && !get_bit(occupancyBitboards[2], f1) && !get_bit(occupancyBitboards[2], g1)) {
            moveList.addMove(encode_move(e1, g1, white, king, false, false, false, true, false, false, noPiece));
        }
        if (get_bit(castleRights, 1) && !get_bit(occupancyBitboards[2], d1) && 
            !get_bit(occupancyBitboards[2], c1) && !get_bit(occupancyBitboards[2], b1)) {
            moveList.addMove(encode_move(e1, c1, white, king, false, false, false, true, false, false, noPiece));
        }
    } else {
        if (get_bit(castleRights, 2) && !get_bit(occupancyBitboards[2], f8) && !get_bit(occupancyBitboards[2], g8)) {
            moveList.addMove(encode_move(e8, g8, black, king, false, false, false, true, false, false, noPiece));
        }
        if (get_bit(castleRights, 3) && !get_bit(occupancyBitboards[2], d8) && 
            !get_bit(occupancyBitboards[2], c8) && !get_bit(occupancyBitboards[2], b8)) {
            moveList.addMove(encode_move(e8, c8, black, king, false, false, false, true, false, false, noPiece));
        }
    }
}


void Board::generateMoves() {
    moveList = MoveList();  // Clear the move list
    
    for (int piece = pawn; piece <= king; piece++) {
        uint64_t pieceBB = pieceBitboards[sideToMove][piece];
        
        while (pieceBB) {
            int square = getLSBIndex(pieceBB);
            
            switch (piece) {
                case pawn:
                    generatePawnMoves(square);
                    break;
                case knight:
                    generateKnightMoves(square);
                    break;
                case bishop:
                    generateBishopMoves(square);
                    break;
                case rook:
                    generateRookMoves(square);
                    break;
                case queen:
                    generateQueenMoves(square);
                    break;
                case king:
                    generateKingMoves(square);
                    break;
            }
            
            pop_bit(pieceBB, square);
        }
    }
}

// Makes a move by updating the board accordingly
bool Board::makeMove(int move)
{
    // Save copy of board
    Board original;
    memcpy(&original, this, sizeof(Board));

    // Move info
    int sourceSquare = get_move_source(move);
    int targetSquare = get_move_target(move);
    int piece = get_move_piece(move);
    int side = get_move_colour(move);

    // Make all the updates to board variables
    
    // Update piece bitboard
    pop_bit(this->pieceBitboards[side][piece], sourceSquare);
    set_bit(this->pieceBitboards[side][piece], targetSquare);

    // Update occupancy bitboard
    pop_bit(this->occupancyBitboards[side], sourceSquare);
    set_bit(this->occupancyBitboards[side], targetSquare);
    pop_bit(this->occupancyBitboards[2], sourceSquare);
    set_bit(this->occupancyBitboards[2], targetSquare);

    // Update zobrist hash for piece-square key
    this->zobristHash ^= pieceSquareKeys[side][piece][sourceSquare];
    this->zobristHash ^= pieceSquareKeys[side][piece][targetSquare];

    // if move was capture, remove the opposing side's piece from bitboard
    if(get_move_capture(move) && !get_move_enpassant(move)){
        for(int i = 0; i < 6; i++){
            if(get_bit(this->pieceBitboards[1-side][i], targetSquare)){
                pop_bit(this->pieceBitboards[1-side][piece], targetSquare);
                this->zobristHash ^= pieceSquareKeys[1-side][piece][targetSquare];
                break;
            }
        }
        pop_bit(this->occupancyBitboards[1-side], targetSquare);
    }
    // enpassant
    else {
        (side == white) ? pop_bit(this->pieceBitboards[black][pawn], targetSquare + 8) : pop_bit(this->pieceBitboards[white][pawn], targetSquare - 8);
        zobristHash ^= pieceSquareKeys[1-side][pawn][side ? targetSquare + 8 : targetSquare - 8];
    }

    zobristHash ^= enpassantKeys[enpassantSquare % 8];
    enpassantSquare = noSquare;

    // if move was double pawn push, set enpassant square
    if(piece == pawn && abs(sourceSquare-targetSquare) > 8){
        (side == white) ? enpassantSquare = targetSquare + 8 : enpassantSquare = targetSquare - 8;
        zobristHash ^= enpassantKeys[enpassantSquare % 8];
    }

    // Update side to move, and zobrist hash accordingly
    this->sideToMove = 1-side;
    this->zobristHash ^= sideToMoveKey;

    // handle promotions
    if(get_move_promoted(move)){
        pop_bit(this->pieceBitboards[side][pawn], targetSquare);
        set_bit(this->pieceBitboards[side][get_move_promotedType(move)], targetSquare);

        this->zobristHash ^= pieceSquareKeys[side][pawn][targetSquare];
        this->zobristHash ^= pieceSquareKeys[side][get_move_promotedType(move)][targetSquare];
    }

    // Handle castling
    // in the board object variable. castling is 0bxxxx = qkQK
    // in zobrist keys array it is {K, Q, k, q}
    if(get_move_castling(move)){
        switch(targetSquare){
            case c8: pop_bit(pieceBitboards[side][rook], a8); set_bit(pieceBitboards[side][rook], d8); zobristHash ^= pieceSquareKeys[side][rook][a8];zobristHash ^= pieceSquareKeys[side][rook][d8];break;
            case c1: pop_bit(pieceBitboards[side][rook], a1); set_bit(pieceBitboards[side][rook], d1); zobristHash ^= pieceSquareKeys[side][rook][a1];zobristHash ^= pieceSquareKeys[side][rook][d1];break;
            case h8: pop_bit(pieceBitboards[side][rook], h8); set_bit(pieceBitboards[side][rook], f8); zobristHash ^= pieceSquareKeys[side][rook][h8];zobristHash ^= pieceSquareKeys[side][rook][f8];break;
            case h1: pop_bit(pieceBitboards[side][rook], h1); set_bit(pieceBitboards[side][rook], f1); zobristHash ^= pieceSquareKeys[side][rook][h1];zobristHash ^= pieceSquareKeys[side][rook][f1];break;
        }
        zobristHash ^= castleRightKeys[side];
        zobristHash ^= castleRightKeys[side+1];
    }
    if(piece == king){
        castleRights &= side ? ~(12) : ~(3);
    }
    if(piece == rook && sourceSquare == a1) castleRights &= 0b1101; // White queenside rook
    if(piece == rook && sourceSquare == h1) castleRights &= 0b1110; // White kingside rook
    if(piece == rook && sourceSquare == a8) castleRights &= 0b0111; // Black queenside rook
    if(piece == rook && sourceSquare == h8) castleRights &= 0b1011; // Black kingside rook

    // if our king is in check after the move, it was illegal. restore position and return false.
    if(isSquareAttacked(1-sideToMove, *this, getLSBIndex(this->pieceBitboards[1-sideToMove][king]))){
        memcpy(this, &original, sizeof(Board));
        return false;
    }
    // else return true
    return true;
}