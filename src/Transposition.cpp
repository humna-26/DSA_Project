#include "Transposition.h"

TTEntry::TTEntry(){
    zobristHash = 0;
    evaluation = 0;
    valid = false;
}

TTEntry::TTEntry(uint64_t z, int e){
    zobristHash = z;
    evaluation = e;
    valid = true;
}

TranspositonTable::TranspositonTable(int s){
    tableSize = s;
    table = new TTEntry[s];
}

void TranspositonTable::store(TTEntry entry){
    table[entry.zobristHash % tableSize] = entry;
}

TTEntry* TranspositonTable::search(uint64_t hash){
    int index = hash % tableSize;
    if(table[index].zobristHash == hash && table[index].valid)
        return &(table[index]);
    return nullptr;
}