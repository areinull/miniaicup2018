#pragma once

#ifndef STRATEGY_ENEMYPREDICTOR_H
#define STRATEGY_ENEMYPREDICTOR_H

#include "V2d.h"


class EnemyPredictor {
public:
    EnemyPredictor(const V2d &pos, float mass, unsigned int tick);
    void update(const V2d &pos, float mass, unsigned int tick);
    unsigned int getTickSeen() const;
    float getMass() const;
    V2d estimatePos(unsigned int deltaTick) const;

private:
    unsigned int tickSeen_ = 0; // 0 is invalid value
    V2d pos_;
    V2d vel_;
    float mass_;
};


#endif //STRATEGY_ENEMYPREDICTOR_H
