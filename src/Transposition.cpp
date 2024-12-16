#include "Transposition.h"
#include "SearchUtil.h"
#include <iostream>

using namespace std;

TTEntry::TTEntry(){
    zobristHash = 0;
    score = 0;
    depth = 0;
    flags = 0;
}

TTEntry::TTEntry(uint64_t z, int e, int d, int f){
    zobristHash = z;
    score = e;
    depth = d;
    flags = f;
}

TranspositonTable::TranspositonTable(){
    tableSize = (ttSizeMB * 1024 * 1024) / sizeof(TTEntry);
    table = new TTEntry[tableSize];
}

void TranspositonTable::store(int score, int depth, uint64_t hash, int flag, int ply){
    TTEntry *entry = &(table[hash % tableSize]);

    // adjust for mating scores
    if(score > mateLower) score += ply;
    if(score < -mateLower) score -= ply;

    entry->score = score;
    entry->depth = depth;
    entry->flags = flag;
    entry->zobristHash = hash;
}

int TranspositonTable::search(uint64_t hash, int alpha, int beta, int depth, int ply){
    TTEntry *entry = &table[hash % tableSize];

    int score = entry->score;

    // adjust for mating scores
    if(score < mateLower) score += ply;
    if(score > -mateLower) score -= ply;

    if((entry->zobristHash == hash) && (entry->depth >= depth)){
        if(entry->flags == entryExact){
            return score;
        }
        if((entry->flags == entryAlpha) && (score <= alpha)){
            return alpha;
        }
        if((entry->flags == entryBeta) && (score >= beta)){
            return beta;
        }
    }

    // if nothing found, return a value that will be outside the possible eval range
    return 2100000000;
}

void TranspositonTable::clear(){
    for(int i = 0; i < tableSize; i++){
        table[i].zobristHash = 0;
        table[i].flags = 0;
        table[i].depth = 0;
        table[i].score = 0;
    }
}

TranspositonTable tt;