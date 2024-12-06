#include <iostream>
#include <cstdint>
#include "PieceUtil.h"
#include "Board.h"
#include "MoveUtil.h"
#include "Transposition.h"
using namespace std;

static char startpos[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main()
{
    initAttackMaps();

    int sizeMB = 64;
    cout << "Enter size: ";
    cin >> sizeMB;
    int nEntries = (sizeMB * 1024 * 1024) / sizeof(TTEntry);

    TranspositonTable tt = TranspositonTable(nEntries);

    srand(time(0));

    uint64_t *arr1 = new uint64_t[nEntries];
    int *arr2 = new int[nEntries];

    int wrong = 0;
    int empty = 0;

    for (int i = 0; i < nEntries; i++) {
        arr1[i] = (static_cast<uint64_t>(rand()) << 32) | rand();
        arr2[i] = rand() % 20001 - 10000;

        tt.store(TTEntry(arr1[i], arr2[i]));

        TTEntry *en = tt.search(arr1[i]);
        if(en == nullptr)
            empty++;
        else if(en->evaluation != arr2[i])
            wrong++;
    }

    cout << "Total: " << nEntries << endl;
    cout << "Wrong Eval: "<< wrong << endl;
    cout << "Null (collision): " << empty << endl << endl;

    wrong = 0;
    empty = 0;

    for(int i = 0; i < nEntries; i++){
        TTEntry *en = tt.search(arr1[i]);
        if(en == nullptr)
            empty++;
        else if(en->evaluation != arr2[i])
            wrong++;
    }

    cout << "Total: " << nEntries << endl;
    cout << "Wrong Eval: "<< wrong << endl;
    cout << "Null (collision): " << empty << endl << endl;

    cin.ignore();
    cin.get();

    return 0;
}