#include "V2d.h"
#include <ostream>

V2d operator*(float k, const V2d &rhs) {
    return {rhs.x * k, rhs.y * k};
}

std::ostream& operator<<(std::ostream& os, const V2d &v) {
    os << '(' << v.x << ';' << v.y << ')';
    return os;
}
