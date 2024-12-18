#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <chrono>
#include <limits.h>
#include <algorithm>

class TimeManager {
private:
    std::chrono::steady_clock::time_point startTime;
    int allocatedTime;  // Time allocated for this move in milliseconds
    bool stopped;
    
    // Time allocation constants
    static constexpr double MOVE_HORIZON = 1.0;  // Plan for this many moves ahead
    static constexpr double TIME_SAFETY_MARGIN = 1.0;  // Use 90% of available time at most

public:
    TimeManager();
    void init(int wtime, int btime, int winc, int binc, int movestogo, bool isWhite);
    void start();
    void stop();
    bool shouldStop();
    int getElapsedTime() const;
    int getAllocatedTime() const;
    
    // Static function to get current time
    static int64_t getCurrentTimeMs();
};

extern TimeManager timeManager;

#endif