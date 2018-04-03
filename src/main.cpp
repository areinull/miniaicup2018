#include "../nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <unordered_set>

#include "Field.h"
#include "FoodInfluence.h"
#include "RandomInfluence.h"
#include "EnemyInfluence.h"

using json = nlohmann::json;

class Strategy {
public:
    void run() {
//        std::ofstream dbg_out("out");
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
//            dbg_out << "==== " << curTick_ << " ====\n" << data << std::endl;
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

        if (!objects.empty()) {
            std::unordered_set<unsigned int> foodToDelete;
            for (auto &obj : objects) {
                if (obj["T"] == "F") {
                    const V2d pos{obj["X"].get<float>(), obj["Y"].get<float>()};
                    const auto id = FoodInfluence::getId(pos);
                    auto it = food_.find(id);
                    if (it == food_.end()) {
                        food_.emplace(id, FoodInfluence{pos, curTick_});
                    } else {
                        if (it->second.isDecayed(curTick_)) {
                            foodToDelete.insert(id);
                        } else {
                            it->second.updateTick(curTick_);
                        }
                    }
                }
            }
            for (auto it = food_.begin(); it != food_.end();) {
                if (foodToDelete.count(it->first)) {
                    it = food_.erase(it);
                } else {
                    ++it;
                }
            }
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
                if (obj["T"] == "P") {
                    f_->applyInfluence(EnemyInfluence(mine, obj));
                }
            }
        }

        for (auto it = food_.begin(); it != food_.end();) {
            // check if we eat it
            bool eaten = false;
            for (const auto &mpart: mine) {
                if ((V2d{mpart["X"].get<float>(), mpart["Y"].get<float>()} - it->second.getPos()).getNormSq() <
                    mpart["R"].get<float>() * mpart["R"].get<float>()) {
                    eaten = true;
                    break;
                }
            }
            if (eaten) {
                it = food_.erase(it);
            } else {
                f_->applyInfluence(it->second);
                ++it;
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
    std::unordered_map<unsigned int, FoodInfluence> food_;
};

int main() {
    Strategy strategy;
    strategy.run();
    return 0;
}