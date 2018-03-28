#include "../nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;
using json = nlohmann::json;

struct Strategy {
    enum class Quadrant {
        LU,
        RU,
        LL,
        RL
    };

    void run() {
        string data;
        cin >> data;
        auto config = json::parse(data);

        std::srand(time(nullptr));

        while (true) {
            cin >> data;
            auto parsed = json::parse(data);
            auto command = on_tick(parsed, config);
            cout << command.dump() << endl;
        }
    }

    json on_tick(const json &data, const json &config) {
        auto mine = data["Mine"];
        auto objects = data["Objects"];
        if (!mine.empty()) {
            auto first = mine[0];
            auto food = find_food(objects);
            if (!food.empty()) {
                return {{"X",     food["X"]},
                        {"Y",     food["Y"]},
                        {"Split", rand() % 100 > 95}};
            } else {
                const int cx = config["GAME_WIDTH"].get<int>() / 2;
                const int cy = config["GAME_HEIGHT"].get<int>() / 2;
                const float mx = first["X"].get<float>();
                const float my = first["Y"].get<float>();

                Quadrant q;
                if (mx < cx && my < cy) q = Quadrant::LU;
                else if (mx < cx && my >= cy) q = Quadrant::LL;
                else if (mx >= cx && my < cy) q = Quadrant::RU;
                else q = Quadrant::RL;

                float x, y;
                switch (q) {
                    case Quadrant::LU:
                        x = 0.f;
                        y = config["GAME_HEIGHT"].get<int>();
                        break;
                    case Quadrant::LL:
                        x = config["GAME_WIDTH"].get<int>();
                        y = config["GAME_HEIGHT"].get<int>();
                        break;
                    case Quadrant::RL:
                        x = config["GAME_WIDTH"].get<int>();
                        y = 0.f;
                        break;
                    case Quadrant::RU:
                        x = y = 0.f;
                        break;
                }
                return {{"X",     x},
                        {"Y",     y},
                        {"Debug", "No food"}};
            }
        }
        return {{"X",     0},
                {"Y",     0},
                {"Debug", "Died"}};
    }

    template<class T>
    json find_food(const T &objects) {
        for (auto &obj : objects) {
            if (obj["T"] == "F") {
                return obj;
            }
        }
        return json({});
    }
};

int main() {
    Strategy strategy;
    strategy.run();
    return 0;
}