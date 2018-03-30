#include "EnemyInfluence.h"

EnemyInfluence::EnemyInfluence(const nlohmann::json &mine, const nlohmann::json &enemy)
        : enemyPos_{enemy["X"].get<float>(), enemy["Y"].get<float>()}, enemyPotential_(0.f) {
    float myMinMass = 0.f;
    for (auto &&mpart: mine) {
        myMinMass = std::min(myMinMass, mpart["M"].get<float>());
    }
    const auto enemyMass = enemy["M"].get<float>();
    if (myMinMass > enemyMass * 1.5f) {
        enemyPotential_ = -3.f;
    } else if (myMinMass < enemyMass) {
        enemyPotential_ = 10.f;
    }
}

float EnemyInfluence::probe(const V2d &v) const {
    return enemyPotential_ / ((v - enemyPos_).getNormSq() + 1e-6f);
}
