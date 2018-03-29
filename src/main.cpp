#include "../nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Field.h"
#include "SimpleGradient.h"
#include "FoodInfluence.h"
#include "RandomInfluence.h"

using namespace std;
using json = nlohmann::json;

struct Strategy {
    unsigned int curTick_ = 0;
    std::unique_ptr<Field> f_;
    std::unique_ptr<RandomInfluence> randomInfluence_;

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
            auto command = on_tick(parsed, config);
            cout << command.dump() << endl;
            ++curTick_;
        }
    }

    json on_tick(const json &data, const json &config) {
        f_->reset();
        if (curTick_%50 == 0) {
            randomInfluence_->update();
        }
        auto mine = data["Mine"];
        auto objects = data["Objects"];
        if (!mine.empty()) {
            auto first = mine[0];
            for (auto &obj : objects) {
                if (obj["T"] == "F") {
                    f_->applyInfluence(FoodInfluence({obj["X"].get<int>(), obj["Y"].get<int>()}));
                }
            }
            f_->applyInfluence(*randomInfluence_);
            const auto dst = f_->getMin();
            return {{"X", dst.x},
                    {"Y", dst.y},
                    {"Split", rand() % 100 > 95}};
        }
        return {{"X",     0},
                {"Y",     0},
                {"Debug", "Died"}};
    }
};

int main() {
    Strategy strategy;
    strategy.run();
    return 0;
}