#include "EnemyPredictor.h"

EnemyPredictor::EnemyPredictor(const V2d &pos, float mass, float r, unsigned int tick):
    tickSeen_(tick), pos_(pos), vel_{0.f, 0.f}, mass_(mass), radius_(r) {

}

void EnemyPredictor::update(const V2d &pos, float mass, float r, unsigned int tick) {
    vel_ = (pos - pos_)/ static_cast<float>(tick - tickSeen_);
    pos_ = pos;
    tickSeen_ = tick;
    mass_ = mass;
    radius_ = r;
}

unsigned int EnemyPredictor::getTickSeen() const {
    return tickSeen_;
}

V2d EnemyPredictor::estimatePos(unsigned int deltaTick) const {
    return pos_ + vel_*deltaTick;
}

float EnemyPredictor::getMass() const {
    return mass_;
}

float EnemyPredictor::getRadius() const {
    return radius_;
}
