#include "FoodInfluence.h"

namespace {
    constexpr unsigned int decayTime = 200;
}

FoodInfluence::FoodInfluence(const V2d &pos, unsigned int tick)
        : pos_(pos), tickSeen_(tick) {

}

float FoodInfluence::probe(const V2d &v) const {
    return -1.f / ((v - pos_).getNormSq() + 1e-6f);
}

bool FoodInfluence::isDecayed(unsigned int tick) const {
    return tick > tickSeen_ + decayTime;
}

void FoodInfluence::updateTick(unsigned int tick) {
    tickSeen_ = tick;
}

unsigned int FoodInfluence::getId(const V2d &pos) {
    return static_cast<unsigned int>(pos.x) +
           static_cast<unsigned int>(pos.y) * 10000;
}
