#pragma once

#ifndef STRATEGY_V2D_H
#define STRATEGY_V2D_H

#include <iosfwd>
#include <cmath>

struct V2d {
    float x;
    float y;

    bool operator==(const V2d &o) const {
        return x == o.x && y == o.y;
    }

    bool operator==(V2d &&o) const {
        return x == o.x && y == o.y;
    }

    bool operator!=(const V2d &o) const {
        return !operator==(o);
    }

    bool operator!=(V2d &&o) const {
        return !operator==(o);
    }

    V2d operator+(const V2d &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    V2d operator-(const V2d &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    V2d &operator+=(const V2d &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    V2d &operator-=(const V2d &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    V2d operator*(float k) const {
        return {x * k, y * k};
    }

    V2d operator/(float k) const {
        return {x / k, y / k};
    }

    V2d &operator*=(float k) {
        x *= k;
        y *= k;
        return *this;
    }

    V2d &operator/=(float k) {
        x /= k;
        y /= k;
        return *this;
    }

    float getNormSq() const {
        return x * x + y * y;
    }

    float getNorm() const {
        return std::sqrt(getNormSq());
    }

    V2d &normalize() {
        const auto n = getNorm();
        x /= n;
        y /= n;
        return *this;
    }

    V2d unit() const {
        return operator/(getNorm());
    }

    V2d rotate(float rad) const {
        const auto s = std::sin(rad);
        const auto c = std::cos(rad);
        return {x * c - y * s, x * s + y * c};
    }

    float operator*(const V2d &rhs) const {
        return x*rhs.x + y*rhs.y;
    }

    float crossZ(const V2d &rhs) const {
        return x*rhs.y - y*rhs.x;
    }
};

V2d operator*(float k, const V2d &rhs);
std::ostream& operator<<(std::ostream& os, const V2d &v);

#endif //STRATEGY_V2D_H
