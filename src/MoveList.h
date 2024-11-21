#ifndef MOVELIST_H
#define MOVELIST_H

class MoveList{
    public:
        int moves[240];
        int count;

        MoveList();
        void addMove(int move);
        void printList();
};

#endif