#pragma once

struct PadState {
    float moveX{}, moveY{};
    float lookX{}, lookY{};
    bool jump{}, magic{}, cupcake{}, start{};
};

class XenonInput {
public:
    void init();
    PadState poll();
private:
    unsigned short previousButtons_{};
};
