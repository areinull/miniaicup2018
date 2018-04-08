#include "ScopeTimer.h"

ScopeTimer::ScopeTimer()
        : start_(std::chrono::steady_clock::now()) {

}

unsigned int ScopeTimer::getDurationMs() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_).count();
}
