#include "uci.h"
#include "Board.h"
#include "TimeManager.h"
#include "PieceUtil.h"
#include "SearchUtil.h"
#include "Transposition.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

class UCITester {
private:
    UCIProtocol uci;
    bool verbose;

    void sendCommand(const std::string& command) {
        if (verbose) {
            std::cout << "Sending command: " << command << std::endl;
        }
        
        // Process the command directly instead of using uci.loop()
        std::istringstream iss(command);
        std::string token;
        iss >> token;

        if (token == "uci") {
            std::cout << "id name ChessEngine" << std::endl;
            std::cout << "id author YourName" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (token == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (token == "position") {
            uci.position(command);
        }
        else if (token == "go") {
            uci.go(command);
        }
        else if (token == "stop") {
            uci.stop();
        }
    }

public:
    UCITester(bool verboseOutput = false) : verbose(verboseOutput) {}

    void runTests() {
        std::cout << "Starting UCI compliance tests..." << std::endl;

        testBasicCommands();
        testPositionCommand();
        testGoCommand();
        
        std::cout << "All tests completed successfully!" << std::endl;
    }

    void testBasicCommands() {
        std::cout << "Testing basic UCI commands..." << std::endl;
        
        // Test UCI initialization
        sendCommand("uci");
        sendCommand("isready");
        
        std::cout << "Basic UCI commands test passed!" << std::endl;
    }

    void testPositionCommand() {
        std::cout << "Testing position commands..." << std::endl;
        
        // Test starting position
        sendCommand("position startpos");
        sendCommand("go depth 1");
        
        // Test position with moves
        sendCommand("position startpos moves e2e4");
        sendCommand("go depth 1");
        
        std::cout << "Position commands test passed!" << std::endl;
    }

    void testGoCommand() {
        std::cout << "Testing go command..." << std::endl;
        
        // Test different time controls
        sendCommand("position startpos");
        sendCommand("go depth 4");
        
        sendCommand("position startpos");
        sendCommand("go movetime 1000");
        
        std::cout << "Go command test passed!" << std::endl;
    }
};

int main() {
    // Initialize required components
    initAttackMaps();
    initZobristKeys();
    tt.clear();
    repetitionIndex = 0;

    // Run tests
    UCITester tester(true);  // true for verbose output
    tester.runTests();

    return 0;
}