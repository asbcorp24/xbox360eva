#pragma once

#include "Dungeon.h"

#include <cstdint>

struct EvaState {
    int health{10};
    int maxHealth{10};
    int cupcakes{2};
    int stars{0};
    int magic{1};
};

class Game {
public:
    explicit Game(std::uint32_t seed);

    void newRun();
    bool move(int dx, int dy);
    void useMagic();
    void eatCupcake();
    void printStatus() const;
    bool isRunning() const noexcept { return running_; }

private:
    void enterRoom(Room& room);
    void resolveRoom(Room& room);
    void nextFloor();

    std::uint32_t seed_;
    Dungeon dungeon_;
    EvaState eva_;
    int floor_{1};
    int x_{0};
    int y_{0};
    bool running_{true};
};
