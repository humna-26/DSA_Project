#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <cstdint>
#include <cstring>

enum {
    entryExact, entryAlpha, entryBeta
};

constexpr int ttSizeMB = 64;

class TTEntry{
    public:
        uint64_t zobristHash;
        int score, depth, flags;

        TTEntry();
        TTEntry(uint64_t z, int s, int d, int f);
};

class TranspositonTable{
    public:
        int tableSize;
        TTEntry *table;

        TranspositonTable();

        void store(int score, int depth, uint64_t hash, int flag, int ply);
        int search(uint64_t hash, int alpha, int beta, int depth, int ply);
        void clear();
};

extern TranspositonTable tt;

#endif