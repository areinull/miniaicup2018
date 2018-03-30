#pragma once

#ifndef STRATEGY_RANDOMINFLUENCE_H
#define STRATEGY_RANDOMINFLUENCE_H

#include "Influence.h"
#include "V2d.h"

class RandomInfluence : public Influence {
public:
    RandomInfluence(float w, float h);
    float probe(const V2d &v) const override;
    void update();
    const V2d& getDst() const { return dst_; }

private:
    const float w_;
    const float h_;
    const float rSq_;
    V2d dst_;
};


#endif //STRATEGY_RANDOMINFLUENCE_H
