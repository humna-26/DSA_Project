#include <iostream>
#include <cstdint>
#include "uci.h"
#include "board.h"
#include "SearchUtil.h"
using namespace std;

UCIProtocol uci;

int main(){

    uci.loop();
    
    return 0;
}