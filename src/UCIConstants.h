#ifndef UCI_CONSTANTS_H
#define UCI_CONSTANTS_H

namespace UCI {
    constexpr int MAX_MOVES = 5000;
    constexpr int MAX_PLY = 128;
    constexpr int DEFAULT_HASH_SIZE = 64;
    constexpr int DEFAULT_DEPTH = 6;
    
    // UCI option names
    const char* const OPTION_HASH = "Hash";
    const char* const OPTION_THREADS = "Threads";
    const char* const OPTION_PONDER = "Ponder";
}

#endif