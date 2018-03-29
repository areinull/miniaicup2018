#pragma once

#ifndef STRATEGY_INFLUENCE_H
#define STRATEGY_INFLUENCE_H

class V2d;

struct Influence {
public:
    virtual ~Influence() = default;

    virtual float probe(const V2d &v) const = 0;
};

#endif //STRATEGY_INFLUENCE_H
