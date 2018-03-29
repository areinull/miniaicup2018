#include <fstream>
#include "Field.h"
#include "V2d.h"
#include "Influence.h"

Field::Field(float width, float height, float step)
        : w_(width), h_(height), s_(step), f_(static_cast<unsigned int>(width * height / step / step), 0) {
}

float Field::operator()(const V2d &v) const {
    return f_.at(static_cast<unsigned int>(v.x/s_) +
                 static_cast<unsigned int>(v.y/s_)*static_cast<unsigned int>(w_/s_));
}

float Field::operator()(V2d &&v) const {
    return f_.at(static_cast<unsigned int>(v.x/s_) +
                 static_cast<unsigned int>(v.y/s_)*static_cast<unsigned int>(w_/s_));
}

float &Field::operator()(const V2d &v) {
    return f_.at(static_cast<unsigned int>(v.x/s_) +
                 static_cast<unsigned int>(v.y/s_)*static_cast<unsigned int>(w_/s_));
}

float &Field::operator()(V2d &&v) {
    return f_.at(static_cast<unsigned int>(v.x/s_) +
                 static_cast<unsigned int>(v.y/s_)*static_cast<unsigned int>(w_/s_));
}

void Field::reset() {
    std::fill(f_.begin(), f_.end(), 0.f);
}

Field &Field::applyInfluence(const Influence &influence) {
    for (float y = s_ / 2; y < h_; y += s_) {
        for (float x = s_ / 2; x < w_; x += s_) {
            const V2d cur{x, y};
            operator()(cur) += influence.probe(cur);
        }
    }
    return *this;
}

V2d Field::getMin() const {
    float min = f_[0];
    unsigned int minIdx = 0;
    for (unsigned int i = 0; i < f_.size(); ++i) {
        if (f_[i] < min) {
            min = f_[i];
            minIdx = i;
        }
    }
    const float xi = minIdx % static_cast<unsigned int>(w_ / s_);
    const float yi = minIdx / static_cast<unsigned int>(w_ / s_);
    return {(xi + 0.5f) * s_, (yi + 0.5f) * s_};
}
