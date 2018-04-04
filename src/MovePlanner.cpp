#include "MovePlanner.h"

MovePlanner::MovePlanner(const nlohmann::json &config)
        : config_(config) {

}

V2d MovePlanner::plan(const nlohmann::json &mpart, const V2d &dst) const {
    return dst;
}
