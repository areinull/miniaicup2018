#include "VirusInfluence.h"

float VirusInfluence::virusRadius;

VirusInfluence::VirusInfluence(const nlohmann::json &mine, const nlohmann::json &virus)
        : pos_{virus["X"].get<float>(), virus["Y"].get<float>()} {
    float mineMaxMass = 0.f;
    for (const auto &mpart : mine) {
        mineMaxMass = std::max(mineMaxMass, mpart["M"].get<float>());
        mineMaxRadius_ = std::max(mineMaxRadius_, mpart["R"].get<float>());
    }
    if (mineMaxMass >= 120.f) {
        active_ = true;
    }
}

float VirusInfluence::probe(const V2d &v) const {
    if (active_) {
        const float dangerRadius = virusRadius + mineMaxRadius_;
        if ((v - pos_).getNormSq() < dangerRadius * dangerRadius) {
            return potential_;
        }
    }
    return 0.f;
}
