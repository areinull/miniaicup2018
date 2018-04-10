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
    void requestUpdate() { forceUpdate_ = true; }
    bool updateRequested() const { return forceUpdate_; }

private:
    const float w_;
    const float h_;
    const float rSq_;
    V2d dst_;
    bool forceUpdate_ = false;
};


#endif //STRATEGY_RANDOMINFLUENCE_H
