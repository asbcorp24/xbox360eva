#include "Dungeon.h"

#include <algorithm>
#include <random>

Dungeon::Dungeon(std::uint32_t seed) : seed_(seed) {}

void Dungeon::generate(int floorNumber) {
    rooms_.clear();
    std::mt19937 rng(seed_ + static_cast<std::uint32_t>(floorNumber * 7919));

    int x = 0;
    int y = 0;
    rooms_.push_back({x, y, RoomType::Start, true, true});

    const int mainLength = 7 + floorNumber;
    for (int i = 1; i < mainLength; ++i) {
        if ((rng() % 2U) == 0U) {
            ++x;
        } else {
            y += ((rng() % 2U) == 0U) ? 1 : -1;
        }

        if (roomAt(x, y) != nullptr) {
            ++x;
        }

        RoomType type = RoomType::CandyPath;
        if (i == 2) type = RoomType::CatCafe;
        if (i == 4) type = RoomType::GiftRoom;
        if (i == mainLength - 2) type = RoomType::RainbowGarden;
        if (i == mainLength - 1) type = RoomType::CloudQueen;
        rooms_.push_back({x, y, type, false, false});
    }
}

Room* Dungeon::roomAt(int x, int y) {
    auto it = std::find_if(rooms_.begin(), rooms_.end(), [=](const Room& room) {
        return room.x == x && room.y == y;
    });
    return it == rooms_.end() ? nullptr : &(*it);
}

const Room* Dungeon::roomAt(int x, int y) const {
    auto it = std::find_if(rooms_.cbegin(), rooms_.cend(), [=](const Room& room) {
        return room.x == x && room.y == y;
    });
    return it == rooms_.cend() ? nullptr : &(*it);
}

std::string Dungeon::roomName(RoomType type) {
    switch (type) {
        case RoomType::Start: return "Розовая поляна";
        case RoomType::CandyPath: return "Конфетная дорожка";
        case RoomType::CatCafe: return "Кошачье кафе";
        case RoomType::GiftRoom: return "Комната подарков";
        case RoomType::RainbowGarden: return "Радужный сад";
        case RoomType::CloudQueen: return "Дворец Облачной Королевы";
    }
    return "Неизвестная комната";
}
