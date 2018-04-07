#pragma once

#ifndef STRATEGY_VIRUSINFLUENCE_H
#define STRATEGY_VIRUSINFLUENCE_H

#include "Influence.h"
#include "V2d.h"
#include "../nlohmann/json.hpp"

class VirusInfluence : public Influence {
public:
    VirusInfluence(const nlohmann::json &mine, const nlohmann::json &virus);
    float probe(const V2d &v) const override;

    static float virusRadius;

private:
    const V2d pos_;
    bool active_ = false;
    float mineMaxRadius_ = 0.f;

    constexpr static float potential_ = 30.f;
};


#endif //STRATEGY_VIRUSINFLUENCE_H
