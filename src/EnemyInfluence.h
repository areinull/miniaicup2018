#pragma once

#ifndef STRATEGY_ENEMYINFLUENCE_H
#define STRATEGY_ENEMYINFLUENCE_H

#include "Influence.h"
#include "../nlohmann/json.hpp"
#include "V2d.h"

class EnemyPredictor;

class EnemyInfluence : public Influence {
public:
    EnemyInfluence(const nlohmann::json &mine, const EnemyPredictor &enemy);
    float probe(const V2d &v) const override;
    bool isDangerous() const;

private:
    V2d enemyPos_;
    float enemyPotential_;
    float enemyRadius_;
    V2d meDir_;
};


#endif //STRATEGY_ENEMYINFLUENCE_H
