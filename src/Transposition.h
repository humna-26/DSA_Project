#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <cstdint>

class TTEntry{
    public:
        uint64_t zobristHash;
        int evaluation;
        bool valid;

        TTEntry();
        TTEntry(uint64_t z, int e);
};

class TranspositonTable{
    public:
        int tableSize;
        TTEntry *table;

        TranspositonTable(int s);

        void store(TTEntry entry);
        TTEntry *search(uint64_t hash);
};

#endif