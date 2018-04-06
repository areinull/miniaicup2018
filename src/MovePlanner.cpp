#include "MovePlanner.h"

MovePlanner::MovePlanner(const nlohmann::json &config)
        : config_(config) {

}

V2d MovePlanner::plan(const nlohmann::json &mine, const V2d &dst) const {
    if (covered(mine, dst)) {
        return dst;
    }

    int maxMassIdx = 0;
    float maxMass = mine[0]["M"].get<float>();
    for (int i = 1; i < mine.size(); ++i) {
        float mass = mine[i]["M"].get<float>();
        if (mass > maxMass) {
            maxMass = mass;
            maxMassIdx = i;
        }
    }
    V2d vel{mine[maxMassIdx]["SX"].get<float>(), mine[maxMassIdx]["SY"].get<float>()};
    V2d pos{mine[maxMassIdx]["X"].get<float>(), mine[maxMassIdx]["Y"].get<float>()};
    const float velNorm = vel.getNorm();
    if (velNorm < 0.01) {
        return dst;
    }

    vel /= velNorm;
    V2d dirDst = dst - pos;
    const float dirDstNorm = dirDst.getNorm();
    if (dirDstNorm < 4.f) {
        return dst;
    }
    dirDst /= dirDstNorm;

    const float crossProd = vel.crossZ(dirDst);
    const V2d dirDstPerp{-dirDst.y, dirDst.x};

    return pos + (dirDst + crossProd*dirDstPerp)*mine[maxMassIdx]["R"].get<float>()*1.2f;
}

bool MovePlanner::covered(const nlohmann::json &mine, const V2d &dst) const {
    bool res = false;
    for (const auto &mpart : mine) {
        const V2d c{mpart["X"].get<float>(), mpart["Y"].get<float>()};
        const auto r = mpart["R"].get<float>();
        if ((c - dst).getNormSq() < r * r * 1.4f) {
            res = true;
            break;
        }
    }
    return res;
}
