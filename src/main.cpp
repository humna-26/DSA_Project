#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
using namespace std;

int main() {

    initAttackMaps();

    int move = encode_move(e2, e4, white, pawn, false, false, false, false, 7);

    cout << "From: " << get_move_source(move) << endl;
    cout << "To: " << get_move_target(move) << endl;
    cout << "Colour: " << get_move_colour(move) << endl;
    cout << "Type: " << get_move_piece(move) << endl;
    cout << "Promoted: " << get_move_promoted(move) << endl;
    cout << "Capture: " << get_move_capture(move) << endl;
    cout << "Enpassant: " << get_move_enpassant(move) << endl;
    cout << "Castling: " << get_move_castling(move) << endl;
    cout << "Promoted Type: " << get_move_promotedType(move) << endl;

    getchar();
    
    return 0;
}

void perft(int depth){
    // To be filled
}