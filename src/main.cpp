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

using namespace std;
using json = nlohmann::json;

class Strategy {
public:
    void run() {
        string data;
        cin >> data;
        auto config = json::parse(data);

        std::srand(time(nullptr));

        f_ = std::make_unique<Field>(config["GAME_WIDTH"].get<int>(),
                                     config["GAME_HEIGHT"].get<int>(),
                                     4);
        randomInfluence_ = std::make_unique<RandomInfluence>(config["GAME_WIDTH"].get<int>(),
                                                             config["GAME_HEIGHT"].get<int>());

        while (true) {
            cin >> data;
            auto parsed = json::parse(data);
            auto command = on_tick_(parsed, config);
            cout << command.dump() << endl;
            ++curTick_;
        }
    }

private:
    json on_tick_(const json &data, const json &config) {
        f_->reset();
        auto mine = data["Mine"];
        auto objects = data["Objects"];

        if (mine.empty()) {
            return {{"X",     0},
                    {"Y",     0},
                    {"Debug", "Died"}};
        }

        if ((V2d{mine[0]["X"].get<float>(), mine[0]["Y"].get<float>()} - randomInfluence_->getDst()).getNormSq() < 100.f) {
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

        bool shouldSplit = mine[0]["M"].get<float>() > 200.f &&
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