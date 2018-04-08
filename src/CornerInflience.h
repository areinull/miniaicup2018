#pragma once

#ifndef STRATEGY_CORNERINFLIENCE_H
#define STRATEGY_CORNERINFLIENCE_H

#include "Influence.h"
#include "V2d.h"
#include "../nlohmann/json.hpp"

class CornerInflience: public Influence {
public:
    CornerInflience(const nlohmann::json &config);
    float probe(const V2d &v) const override;

private:
    constexpr static float radius_ = 32.f;
    constexpr static float potential_ = 5.f;
    const float maxX_;
    const float maxY_;
};


#endif //STRATEGY_CORNERINFLIENCE_H
