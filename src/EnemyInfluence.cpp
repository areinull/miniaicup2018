#include "EnemyInfluence.h"

EnemyInfluence::EnemyInfluence(const nlohmann::json &mine, const nlohmann::json &enemy)
        : enemyPos_{enemy["X"].get<float>(), enemy["Y"].get<float>()}, enemyPotential_(0.f),
          enemyRadius_(enemy["R"].get<float>()) {
    float myMinMass = std::numeric_limits<float>::max();
    for (auto &mpart: mine) {
        myMinMass = std::min(myMinMass, mpart["M"].get<float>());
    }
    const auto enemyMass = enemy["M"].get<float>();
    if (myMinMass > enemyMass * 1.5f) {
        enemyPotential_ = -10.f;
    } else if (myMinMass < enemyMass * 1.1f) {
        enemyPotential_ = 100.f;
    }
}

float EnemyInfluence::probe(const V2d &v) const {
    return (v - enemyPos_).getNormSq() < 9.f * enemyRadius_ * enemyRadius_ ? enemyPotential_ : 0.f;
}
