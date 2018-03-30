#pragma once

#ifndef STRATEGY_ENEMYINFLUENCE_H
#define STRATEGY_ENEMYINFLUENCE_H

#include "Influence.h"
#include "../nlohmann/json.hpp"
#include "V2d.h"

class EnemyInfluence : public Influence {
public:
    EnemyInfluence(const nlohmann::json &mine, const nlohmann::json &enemy);
    float probe(const V2d &v) const override;

private:
    V2d enemyPos_;
    float enemyPotential_;
};


#endif //STRATEGY_ENEMYINFLUENCE_H
