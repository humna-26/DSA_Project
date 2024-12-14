#include "MoveList.h"
#include "MoveUtil.h"
#include <iostream>
using namespace std;

// constructor to init count. No need to init array
MoveList::MoveList(){
    count = 0;
}

// Function to add a move to the move list
void MoveList::addMove(int move){
    moves[count] = move;
    // increment move count
    count++;
}

// Function to print all moves in the list
void MoveList::printList(){
    for (int i = 0; i < count; i++) 
        cout << printMove(moves[i]) << endl;
}