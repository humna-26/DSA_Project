#ifndef UCI_H
#define UCI_H

#include "Board.h"
#include "TimeManager.h"
#include "UCIConstants.h"
#include <string>

class UCIProtocol {  // Changed from UCI to UCIProtocol
private:
    Board board;
    bool debug;
    bool running;
    
    // Time control variables
    int wtime;
    int btime;
    int winc;
    int binc;
    int movetime;
    int movestogo;
    int depth;
    bool infinite;

    void parsePosition(const std::string& command);
    void parseGo(const std::string& command);
    void setOption(const std::string& command);
    
public:
    UCIProtocol();  // Changed constructor name
    void loop();
    void position(const std::string& command);
    void go(const std::string& command);
    void stop();
    void quit();
};

#endif