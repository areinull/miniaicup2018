#include "EnemyInfluence.h"
#include "EnemyPredictor.h"

EnemyInfluence::EnemyInfluence(const nlohmann::json &mine, const EnemyPredictor &enemy)
        : enemyPotential_(0.f), enemyRadius_(enemy.getRadius()) {
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
    const auto enemyMass = enemy.getMass();
    if (myMinMass > enemyMass * 1.2f) {
        enemyPotential_ = -100.f;
        enemyPos_ = enemy.estimatePos(5);
    } else if (myMinMass * 1.1f < enemyMass) {
        enemyPotential_ = 100.f;
        enemyPos_ = enemy.estimatePos(0);
    }
    meDir_ = enemyPos_ - avgPos;
}

float EnemyInfluence::probe(const V2d &v) const {
    const auto dir = v - enemyPos_;
    float result = 0.f;
    if (enemyPotential_ > 0.f) {
        if (dir.getNormSq() < 16.f * enemyRadius_ * enemyRadius_ || dir * meDir_ > 0.f) {
            result += enemyPotential_;
        }
        // add little attraction in opposite direction
        const auto meDirUnit = meDir_.unit();
        const auto attract = enemyPos_ - 10.f * enemyRadius_ * meDirUnit;
        if ((v - attract).getNormSq() < enemyRadius_*enemyRadius_) {
            result -= 5.f;
        }
        // add attraction gradient in opposite direction (works when attractor is out of field)
        const auto oppositeDist = meDirUnit * dir;
        if (oppositeDist < -enemyRadius_) {
            result += oppositeDist * 0.001f;
        }
    } else {
        if (dir.getNormSq() < std::max(32.f, enemyRadius_ * enemyRadius_)) {
            result += enemyPotential_;
        }
    }
    return result;
}

bool EnemyInfluence::isDangerous() const {
    return enemyPotential_ > 0.f;
}