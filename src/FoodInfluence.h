#pragma once

#ifndef STRATEGY_FOODINFLUENCE_H
#define STRATEGY_FOODINFLUENCE_H

#include "Influence.h"
#include "V2d.h"

class FoodInfluence : public Influence {
public:
    FoodInfluence(const V2d &pos);
    float probe(const V2d &v) const override;

private:
    const V2d pos_;
};


#endif //STRATEGY_FOODINFLUENCE_H
