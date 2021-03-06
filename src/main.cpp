#include "../nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unordered_map>

#include "Field.h"
#include "FoodInfluence.h"
#include "RandomInfluence.h"
#include "EnemyInfluence.h"
#include "VirusInfluence.h"
#include "CornerInflience.h"
#include "MovePlanner.h"
#include "ScopeTimer.h"
#include "EnemyPredictor.h"

using json = nlohmann::json;

class Strategy {
public:
    void run() {
        std::string data;
        std::cin >> data;
        auto config = json::parse(data);

        std::srand(time(nullptr));

        f_ = std::make_unique<Field>(config["GAME_WIDTH"].get<int>(),
                                     config["GAME_HEIGHT"].get<int>(),
                                     4);
        randomInfluence_ = std::make_unique<RandomInfluence>(config["GAME_WIDTH"].get<int>(),
                                                             config["GAME_HEIGHT"].get<int>());
        movePlanner_ = std::make_unique<MovePlanner>(config);
        VirusInfluence::virusRadius = config["VIRUS_RADIUS"].get<float>();
        cornerInfluence_ = std::make_unique<CornerInflience>(config);

        while (true) {
            std::cin >> data;
            auto parsed = json::parse(data);
            auto command = on_tick_(parsed, config);
            std::cout << command.dump() << std::endl;
            ++curTick_;
        }
    }

private:
    json on_tick_(const json &data, const json &config) {
        const ScopeTimer scopeTimer;
        f_->reset();
        const auto &mine = data.count("Mine")? data["Mine"]: json();
        const auto &objects = data.count("Objects")? data["Objects"]: json();

        if (mine.empty()) {
            return {{"X",     0},
                    {"Y",     0},
                    {"Debug", "Died"}};
        }

        for (auto e = enemyPredictors_.begin(); e != enemyPredictors_.end();) {
            if (curTick_ - e->second.getTickSeen() > 20) {
                e = enemyPredictors_.erase(e);
            } else {
                ++e;
            }
        }

        if (!objects.empty()) {
            for (auto &obj : objects) {
                if (obj["T"] != "P")
                    continue;
                const V2d pos{obj["X"].get<float>(), obj["Y"].get<float>()};
                const auto mass = obj["M"].get<float>();
                const auto radius = obj["R"].get<float>();
                auto e = enemyPredictors_.find(obj["Id"].get<std::string>());
                if (e == enemyPredictors_.end()) {
                    enemyPredictors_.emplace(obj["Id"].get<std::string>(), EnemyPredictor{pos, mass, radius, curTick_});
                    continue;
                } else {
                    e->second.update(pos, mass, radius, curTick_);
                }
            }
        }

        // check shoot
        do {
            float maxEnemyMass = -1.f;
            std::string maxEnemyId;
            for (auto &e : enemyPredictors_) {
                const auto mass = e.second.getMass();
                if (mass > maxEnemyMass) {
                    maxEnemyMass = mass;
                    maxEnemyId = e.first;
                }
            }
            if (maxEnemyMass < 0.f)
                break;
            const V2d maxEnemyPos = enemyPredictors_.at(maxEnemyId).estimatePos(5);

            enemySeenTick_ = curTick_;
            auto myMinMass = std::numeric_limits<float>::max();
            V2d vel, minPos;
            for (auto &mpart : mine) {
                const auto mass = mpart["M"].get<float>();
                if (mass < myMinMass) {
                    myMinMass = mass;
                    minPos.x = mpart["X"].get<float>();
                    minPos.y = mpart["Y"].get<float>();
                    vel.x = mpart["SX"].get<float>();
                    vel.y = mpart["SY"].get<float>();
                }
            }
            if (myMinMass / 2.f < maxEnemyMass * 1.3f)
                break;
            if (vel.getNormSq() < 1e-4f)
                break;
            vel.normalize();

            // check current direction
            V2d enemyDir = maxEnemyPos - minPos;
            enemyDir.normalize();
            const auto proj = enemyDir * vel;
            const bool readySplit = proj > 0.9f;

            return {{"X",     maxEnemyPos.x},
                    {"Y",     maxEnemyPos.y},
                    {"Split", readySplit},
                    {"Debug", "tick " + std::to_string(curTick_) +
                              " elapsed (ms) " + std::to_string(scopeTimer.getDurationMs())}};
        } while (false);

        if (randomInfluence_->updateRequested() || movePlanner_->covered(mine, randomInfluence_->getDst())) {
            randomInfluence_->update();
        }

        if (!objects.empty()) {
            for (auto &obj : objects) {
                if (obj["T"] == "F") {
                    f_->applyInfluence(FoodInfluence({obj["X"].get<float>(), obj["Y"].get<float>()}));
                }
            }
        }
        bool enemyVisible = false;
        bool enemyDangerous = false;
        for (auto &e : enemyPredictors_) {
            enemyVisible = true;
            const EnemyInfluence enemyInfluence(mine, e.second);
            enemyDangerous = enemyDangerous || enemyInfluence.isDangerous();
            f_->applyInfluence(enemyInfluence);
        }
        f_->applyInfluence(*randomInfluence_);
        if (enemyVisible) {
            enemySeenTick_ = curTick_;
            if (enemyDangerous) {
                randomInfluence_->requestUpdate();
                f_->applyInfluence(*cornerInfluence_);
            }
            for (auto &obj : objects) {
                if (obj["T"] == "V") {
                    f_->applyInfluence(VirusInfluence(mine, obj));
                }
            }
        }
        const auto dst = movePlanner_->plan(mine, f_->getMin());

        const bool shouldSplit = curTick_ > enemySeenTick_ + 200;

        return {{"X",     dst.x},
                {"Y",     dst.y},
                {"Split", shouldSplit},
                {"Debug", "tick " + std::to_string(curTick_) +
                          " elapsed (ms) " + std::to_string(scopeTimer.getDurationMs())}};
    }


    unsigned int curTick_ = 1;
    unsigned int enemySeenTick_ = 0;
    std::unique_ptr<Field> f_;
    std::unique_ptr<RandomInfluence> randomInfluence_;
    std::unique_ptr<MovePlanner> movePlanner_;
    std::unique_ptr<CornerInflience> cornerInfluence_;
    std::unordered_map<std::string, EnemyPredictor> enemyPredictors_;
};

int main() {
    Strategy strategy;
    try {
        strategy.run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
