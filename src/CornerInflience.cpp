#include "CornerInflience.h"

CornerInflience::CornerInflience(const nlohmann::json &config)
        : maxX_(config["GAME_WIDTH"].get<float>()), maxY_(config["GAME_HEIGHT"].get<float>()) {

}

float CornerInflience::probe(const V2d &v) const {
    float result = 0.f;
    const V2d fromCorners[] = {v, v-V2d{maxX_, 0.f}, v-V2d{0.f, maxY_}, v-V2d{maxX_, maxY_}};
    for (const V2d dir : fromCorners) {
        if (dir.getNormSq() < radius_*radius_) {
            result += potential_;
        }
    }
    return result;
}
