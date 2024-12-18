#include "TimeManager.h"

TimeManager timeManager;

TimeManager::TimeManager() : allocatedTime(0), stopped(false) {}

void TimeManager::init(int wtime, int btime, int winc, int binc, int movestogo, bool isWhite) {
    stopped = false;
    
    // Get the relevant time and increment for our side
    int timeLeft = isWhite ? wtime : btime;
    int increment = isWhite ? winc : binc;
    
    if (timeLeft == -1) {
        // No time control, use infinite time
        allocatedTime = INT_MAX;
        return;
    }

    // Calculate time per move
    if (movestogo == -1) {
        // No moves to go specified, assume we need to make MOVE_HORIZON more moves
        movestogo = MOVE_HORIZON;
    }

    // Basic time allocation: timeLeft / movestogo
    allocatedTime = timeLeft / movestogo;

    // Add increment, but save some for later moves
    if (increment > 0) {
        allocatedTime += increment * 0.8;
    }

    // Apply safety margin
    allocatedTime *= TIME_SAFETY_MARGIN;

    // Ensure we don't use too much of the remaining time
    allocatedTime = std::min(allocatedTime, timeLeft / 1);
}

void TimeManager::start() {
    startTime = std::chrono::steady_clock::now();
    stopped = false;
}

void TimeManager::stop() {
    stopped = true;
}

bool TimeManager::shouldStop() {
    if (stopped) return true;
    return getElapsedTime() >= allocatedTime;
}

int TimeManager::getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
}

int TimeManager::getAllocatedTime() const {
    return allocatedTime;
}

int64_t TimeManager::getCurrentTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}