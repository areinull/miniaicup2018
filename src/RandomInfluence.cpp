#include <cstdlib>
#include "RandomInfluence.h"

RandomInfluence::RandomInfluence(float w, float h)
        : w_(w), h_(h), rSq_(8.f*8.f) {
    update();
}

float RandomInfluence::probe(const V2d &v) const {
    return (v-dst_).getNormSq() < rSq_? -0.01f : 0.f;
}

void RandomInfluence::update() {
    dst_.x = rand()%static_cast<unsigned int>(w_);
    dst_.y = rand()%static_cast<unsigned int>(h_);
}
