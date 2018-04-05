#pragma once

#ifndef STRATEGY_MOVEPLANNER_H
#define STRATEGY_MOVEPLANNER_H

#include "../nlohmann/json.hpp"
#include "V2d.h"

class MovePlanner {
public:
    MovePlanner(const nlohmann::json &config);

    V2d plan(const nlohmann::json &mine, const V2d &dst) const;

private:
    const nlohmann::json &config_;
};


#endif //STRATEGY_MOVEPLANNER_H
