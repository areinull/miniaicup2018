#include "../nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Field.h"
#include "SimpleGradient.h"
#include "FoodInfluence.h"
#include "RandomInfluence.h"
#include "EnemyInfluence.h"

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
            V2d vel;
            for (auto &mpart : mine) {
                const auto mass = mpart["M"].get<float>();
                if (mass < myMinMass) {
                    myMinMass = mass;
                    vel.x = mpart["SX"].get<float>();
                    vel.y = mpart["SY"].get<float>();
                }
            }
            if (myMinMass / 2.f < maxEnemyMass * 1.3f)
                break;

            // check current direction
            V2d enemyDir{maxEnemyX, maxEnemyY};
            enemyDir.normalize();
            const auto proj = enemyDir * vel;
            const bool readySplit = proj > 0.8f;

            return {{"X",     maxEnemyX},
                    {"Y",     maxEnemyY},
                    {"Split", readySplit}};
        } while (false);

        if ((V2d{mine[0]["X"].get<float>(), mine[0]["Y"].get<float>()} - randomInfluence_->getDst()).getNormSq() <
            100.f) {
            randomInfluence_->update();
        }

        if (!objects.empty()) {
            for (auto &obj : objects) {
                if (obj["T"] == "F") {
                    f_->applyInfluence(FoodInfluence({obj["X"].get<float>(), obj["Y"].get<float>()}));
                } else if (obj["T"] == "P") {
                    f_->applyInfluence(EnemyInfluence(mine, obj));
                }
            }
        }
        f_->applyInfluence(*randomInfluence_);
        const auto dst = f_->getMin();

        bool shouldSplit = mine[0]["M"].get<float>() > 400.f &&
                           rand() % 100 > 98;

        return {{"X",     dst.x},
                {"Y",     dst.y},
                {"Split", shouldSplit}};
    }


    unsigned int curTick_ = 0;
    std::unique_ptr<Field> f_;
    std::unique_ptr<RandomInfluence> randomInfluence_;
};

int main() {
    Strategy strategy;
    strategy.run();
    return 0;
}