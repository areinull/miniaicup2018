#pragma once

#ifndef STRATEGY_SCOPETIMER_H
#define STRATEGY_SCOPETIMER_H

#include <chrono>

class ScopeTimer {
public:
    ScopeTimer();
    unsigned int getDurationMs() const;

private:
    std::chrono::steady_clock::time_point start_;
};


#endif //STRATEGY_SCOPETIMER_H
