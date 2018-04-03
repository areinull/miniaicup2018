#pragma once

#ifndef STRATEGY_FOODINFLUENCE_H
#define STRATEGY_FOODINFLUENCE_H

#include "Influence.h"
#include "V2d.h"

class FoodInfluence : public Influence {
public:
    FoodInfluence(const V2d &pos, unsigned int tick);
    float probe(const V2d &v) const override;
    bool isDecayed(unsigned int tick) const;
    void updateTick(unsigned int tick);
    const V2d& getPos() const {
        return pos_;
    }

    static unsigned int getId(const V2d &pos);

private:
    const V2d pos_;
    unsigned int tickSeen_;
};


#endif //STRATEGY_FOODINFLUENCE_H
