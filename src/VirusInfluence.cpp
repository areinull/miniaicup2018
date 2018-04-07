#include "VirusInfluence.h"

float VirusInfluence::virusRadius;

VirusInfluence::VirusInfluence(const nlohmann::json &mine, const nlohmann::json &virus)
        : pos_{virus["X"].get<float>(), virus["Y"].get<float>()} {
    unsigned int idx = 0;
    float mineMaxMass = mine[0]["M"].get<float>();
    for (unsigned int i = 1; i < mine.size(); ++i) {
        const auto mass = mine[i]["M"].get<float>();
        if (mass > mineMaxMass) {
            idx = i;
            mineMaxMass = mass;
        }
    }
    if (mineMaxMass >= 120.f) {
        active_ = true;
        mineMaxRadius_ = mine[idx]["R"].get<float>();
        mvDir_ = {pos_.x - mine[idx]["X"].get<float>(), pos_.y - mine[idx]["Y"].get<float>()};
    }
}

float VirusInfluence::probe(const V2d &v) const {
    if (active_) {
        const float dangerRadius = virusRadius + mineMaxRadius_;
        const auto dir = v - pos_;
        if (dir.getNormSq() < dangerRadius * dangerRadius) {
            return potential_;
        }
        const auto d = mvDir_.unit();
        const V2d n{-d.y, d.x};
        const auto N1 = pos_ + n*virusRadius;
        const auto N2 = pos_ - n*virusRadius;
        if (dir * d > 0.f &&
            (v-N1)*n < 0.f &&
            (v-N2)*n > 0.f) {
            return potential_;
        }
    }
    return 0.f;
}
