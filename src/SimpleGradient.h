#pragma once

#ifndef STRATEGY_SIMPLEGRADIENT_H
#define STRATEGY_SIMPLEGRADIENT_H

#include "Influence.h"

class SimpleGradient: public Influence {
public:
    float probe(const V2d &v) const override;
};


#endif //STRATEGY_SIMPLEGRADIENT_H
