#include "EnemyInfluence.h"

EnemyInfluence::EnemyInfluence(const nlohmann::json &mine, const nlohmann::json &enemy)
        : enemyPos_{enemy["X"].get<float>(), enemy["Y"].get<float>()}, enemyPotential_(0.f),
          enemyRadius_(enemy["R"].get<float>()) {
    V2d avgPos{0.f,0.f};
    float totalMass = 0.f;
    float myMinMass = std::numeric_limits<float>::max();
    for (auto &mpart: mine) {
        const auto mass = mpart["M"].get<float>();
        myMinMass = std::min(myMinMass, mass);
        totalMass += mass;
        avgPos += V2d{mpart["X"].get<float>(), mpart["Y"].get<float>()} * mass;
    }
    avgPos /= totalMass;
    meDir_ = enemyPos_ - avgPos;
    const auto enemyMass = enemy["M"].get<float>();
    if (myMinMass > enemyMass * 1.2f) {
        enemyPotential_ = -100.f;
    } else if (myMinMass * 1.1f < enemyMass) {
        enemyPotential_ = 100.f;
    }
}

float EnemyInfluence::probe(const V2d &v) const {
    const auto dir = v - enemyPos_;
    if (enemyPotential_ > 0.f) {
        if (dir.getNormSq() < 16.f * enemyRadius_ * enemyRadius_ || dir * meDir_ > 0.f) {
            return enemyPotential_;
        }
    } else {
        if (dir.getNormSq() < std::max(32.f, enemyRadius_ * enemyRadius_)) {
            return enemyPotential_;
        }
    }
    return 0.f;
}
