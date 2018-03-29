#pragma once

#ifndef STRATEGY_FIELD_H
#define STRATEGY_FIELD_H

#include <vector>
#include "V2d.h"

class V2d;
class Influence;

class Field {
public:
    Field(float width, float height, float step);

    Field(const Field &) = delete;
    Field(Field &&) = delete;
    Field &operator=(const Field &) = delete;
    Field &operator=(Field &&) = delete;

    float w() const { return w_; }
    float h() const { return h_; }
    float s() const { return s_; }
    float operator()(const V2d &v) const;
    float operator()(V2d &&v) const;
    float& operator()(const V2d &v);
    float& operator()(V2d &&v);
    void reset();

    Field& applyInfluence(const Influence& influence);
    V2d getMin() const;

private:
    const float w_; // width
    const float h_; // height
    const float s_; // step
    std::vector<float> f_; // field
};


#endif //STRATEGY_FIELD_H
