#include "../nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Field.h"
#include "FoodInfluence.h"
#include "RandomInfluence.h"
#include "EnemyInfluence.h"
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

        int maxMassIdx = 0;
        {
            float maxMass = mine[0]["M"].get<float>();
            for (int i = 1; i < mine.size(); ++i) {
                const auto mass = mine[i]["M"].get<float>();
                if (mass > maxMass) {
                    maxMass = mass;
                    maxMassIdx = i;
                }
            }
        }
        if ((V2d{mine[maxMassIdx]["X"].get<float>(), mine[maxMassIdx]["Y"].get<float>()} - randomInfluence_->getDst()).getNormSq() <
            mine[maxMassIdx]["R"].get<float>()*mine[maxMassIdx]["R"].get<float>()*2.f) {
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
        const auto dst = movePlanner_->plan(mine, f_->getMin());

        const bool shouldSplit = !enemyVisible &&
                                 mine[0]["M"].get<float>() > 400.f &&
                                 rand() % 100 > 98;

        return {{"X",     dst.x},
                {"Y",     dst.y},
                {"Split", shouldSplit}};
    }


    unsigned int curTick_ = 0;
    std::unique_ptr<Field> f_;
    std::unique_ptr<RandomInfluence> randomInfluence_;
    std::unique_ptr<MovePlanner> movePlanner_;
};

int main() {
    Strategy strategy;
    strategy.run();
    return 0;
}