#include "../nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Field.h"
#include "FoodInfluence.h"
#include "RandomInfluence.h"
#include "EnemyInfluence.h"
#include "VirusInfluence.h"
#include "MovePlanner.h"

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
        f_->reset();
        const auto &mine = data.count("Mine")? data["Mine"]: json();
        const auto &objects = data.count("Objects")? data["Objects"]: json();

        if (mine.empty()) {
            return {{"X",     0},
                    {"Y",     0},
                    {"Debug", "Died"}};
        }

        // check shoot
        do {
            if (objects.empty())
                break;
            float maxEnemyMass = -1.f;
            float maxEnemyX, maxEnemyY;
            for (auto &obj : objects) {
                if (obj["T"] == "P") {
                    const auto mass = obj["M"].get<float>();
                    if (mass > maxEnemyMass) {
                        maxEnemyMass = mass;
                        maxEnemyX = obj["X"].get<float>();
                        maxEnemyY = obj["Y"].get<float>();
                    }

                }
            }
            if (maxEnemyMass < 0.f)
                break;

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
            V2d enemyDir{maxEnemyX - minPos.x, maxEnemyY - minPos.y};
            enemyDir.normalize();
            const auto proj = enemyDir * vel;
            const bool readySplit = proj > 0.9f;

            return {{"X",     maxEnemyX},
                    {"Y",     maxEnemyY},
                    {"Split", readySplit}};
        } while (false);

        if (movePlanner_->covered(mine, randomInfluence_->getDst())) {
            randomInfluence_->update();
        }

        bool enemyVisible = false;
        if (!objects.empty()) {
            for (auto &obj : objects) {
                if (obj["T"] == "F") {
                    f_->applyInfluence(FoodInfluence({obj["X"].get<float>(), obj["Y"].get<float>()}));
                } else if (obj["T"] == "P") {
                    enemyVisible = true;
                    f_->applyInfluence(EnemyInfluence(mine, obj));
                }
            }
        }
        f_->applyInfluence(*randomInfluence_);
        if (enemyVisible) {
            enemySeenTick_ = curTick_;
            for (auto &obj : objects) {
                if (obj["T"] == "V") {
                    f_->applyInfluence(VirusInfluence(mine, obj));
                }
            }
        }
        const auto dst = movePlanner_->plan(mine, f_->getMin());

        const bool shouldSplit = curTick_ > enemySeenTick_ + 100;

        return {{"X",     dst.x},
                {"Y",     dst.y},
                {"Split", shouldSplit}};
    }


    unsigned int curTick_ = 0;
    unsigned int enemySeenTick_ = 0;
    std::unique_ptr<Field> f_;
    std::unique_ptr<RandomInfluence> randomInfluence_;
    std::unique_ptr<MovePlanner> movePlanner_;
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
