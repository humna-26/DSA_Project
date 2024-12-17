#include "uci.h"
#include "SearchUtil.h"
#include "UCIConstants.h"
#include "TimeManager.h"
#include "MoveUtil.h"
#include "PieceUtil.h"
#include "Board.h"
#include <iostream>
#include <sstream>
#include <string>
#include <climits>

UCIProtocol::UCIProtocol() {
    board = Board();
    debug = false;
    running = false;
    
    // Initialize time control variables
    wtime = -1;
    btime = -1;
    winc = 0;
    binc = 0;
    movetime = -1;
    movestogo = -1;
    depth = UCI::DEFAULT_DEPTH;
    infinite = false;
}

void UCIProtocol::loop() {
    std::string line;
    std::string token;

    // Main UCI loop
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        iss >> token;

        if (token == "uci") {
            std::cout << "id name YourChessEngine" << std::endl;
            std::cout << "id author YourName" << std::endl;
            // Add UCI options here if needed
            std::cout << "uciok" << std::endl;
        }
        else if (token == "debug") {
            iss >> token;
            debug = (token == "on");
        }
        else if (token == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (token == "setoption") {
            setOption(line);
        }
        else if (token == "position") {
            parsePosition(line);
        }
        else if (token == "go") {
            parseGo(line);
        }
        else if (token == "stop") {
            stop();
        }
        else if (token == "quit") {
            quit();
            break;
        }
        else if (token == "ucinewgame") {
            // Reset the board and clear any persistent data (like transposition table)
            board = Board();
            tt.clear();
            repetitionIndex = 0;
        }
    }
}

void UCIProtocol::position(const std::string& command) {
    std::istringstream iss(command);
    std::string token;

    iss >> token; // Skip "position" token

    iss >> token;
    if (token == "startpos") {
        board = Board();
        iss >> token; // Potentially "moves"
    }
    else if (token == "fen") {
        std::string fen;
        // Collect the FEN string
        while (iss >> token && token != "moves") {
            fen += token + " ";
        }
        board = Board();
        board.initFromFen(const_cast<char*>(fen.c_str()));
    }

    // If there are moves, play them
    if (token == "moves") {
        while (iss >> token) {
            board.generateMoves();
            for (int i = 0; i < board.moveList.count; i++) {
                if (printMove(board.moveList.moves[i]) == token) {
                    board.makeMove(board.moveList.moves[i]);
                    break;
                }
            }
        }
    }
}

void UCIProtocol::go(const std::string& command) {
    std::istringstream iss(command);
    std::string token;

    // Reset time control variables
    wtime = -1;
    btime = -1;
    winc = 0;
    binc = 0;
    movetime = -1;
    movestogo = -1;
    depth = UCI::DEFAULT_DEPTH;
    infinite = false;

    iss >> token; // Skip "go" token

    // Parse go parameters
    while (iss >> token) {
        if (token == "infinite") {
            depth = UCI::MAX_PLY;
        }
        else if (token == "depth") {
            iss >> depth;
        }
        else if (token == "movetime") {
            iss >> movetime;
        }
        else if (token == "wtime") {
            iss >> wtime;
        }
        else if (token == "btime") {
            iss >> btime;
        }
        else if (token == "winc") {
            iss >> winc;
        }
        else if (token == "binc") {
            iss >> binc;
        }
        else if (token == "movestogo") {
            iss >> movestogo;
        }
    }

    // Initialize time management
    if (movetime != -1) {
        timeManager.init(movetime, movetime, 0, 0, 1, board.sideToMove == white);
    } else {
        timeManager.init(wtime, btime, winc, binc, movestogo, board.sideToMove == white);
    }

    // Start the search
    auto bestMove = findBestMove(board, depth);
    
    // Output the best move
    std::cout << "bestmove " << printMove(bestMove.first) << std::endl;
}

void UCIProtocol::parsePosition(const std::string& command) {
    std::istringstream iss(command);
    std::string token;
    std::string fen;

    iss >> token; // Skip "position" token

    iss >> token;
    if (token == "startpos") {
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        iss >> token; // Potentially "moves"
    }
    else if (token == "fen") {
        // Collect the FEN string
        while (iss >> token && token != "moves") {
            fen += token + " ";
        }
    }

    // Set up the position
    board = Board();
    board.initFromFen(const_cast<char*>(fen.c_str()));

    // If there are moves, play them
    while (iss >> token) {
        // Convert UCI move format to internal move format and apply it
        // This part needs to be implemented based on your move encoding
        board.generateMoves();
        for (int i = 0; i < board.moveList.count; i++) {
            if (printMove(board.moveList.moves[i]) == token) {
                board.makeMove(board.moveList.moves[i]);
                // Update repetition table
                repetitionIndex++;
                gameHistoryTable[repetitionIndex] = board.zobristHash;
                break;
            }
        }
    }
}

void UCIProtocol::parseGo(const std::string& command) {
    std::istringstream iss(command);
    std::string token;

    // Reset time control variables
    wtime = -1;
    btime = -1;
    winc = 0;
    binc = 0;
    movetime = -1;
    movestogo = -1;
    depth = -1;
    infinite = false;

    iss >> token; // Skip "go" token

    // Parse go parameters
    while (iss >> token) {
        if (token == "infinite") {
            infinite = true;
        }
        else if (token == "wtime") {
            iss >> wtime;
        }
        else if (token == "btime") {
            iss >> btime;
        }
        else if (token == "winc") {
            iss >> winc;
        }
        else if (token == "binc") {
            iss >> binc;
        }
        else if (token == "movestogo") {
            iss >> movestogo;
        }
        else if (token == "depth") {
            iss >> depth;
        }
        else if (token == "movetime") {
            iss >> movetime;
        }
    }
    // Initialize time management
    timeManager.init(wtime, btime, winc, binc, movestogo, board.sideToMove == white);
    
    // Calculate appropriate depth
    int searchDepth;
    if (depth != -1) {
        searchDepth = depth;
    } else if (infinite) {
        searchDepth = UCI::MAX_PLY;
    } else {
        searchDepth = 64;  // Will be limited by time
    }
    
    // Start the search
    auto bestMove = findBestMove(board, searchDepth);
    
    // Output the best move
    std::cout << "bestmove " << printMove(bestMove.first) << std::endl;

}

void UCIProtocol::setOption(const std::string& command) {
    // Implement setting UCI options here
    // For now, we don't have any options to set
}

void UCIProtocol::stop() {
    timeManager.stop();
}

void UCIProtocol::quit() {
    // Cleanup before quitting
    running = false;
}

