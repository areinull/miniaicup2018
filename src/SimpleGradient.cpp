#include "SimpleGradient.h"
#include "V2d.h"

float SimpleGradient::probe(const V2d &v) const {
    return -v.x;
}
