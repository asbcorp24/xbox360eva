#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class RoomType {
    Start,
    CandyPath,
    CatCafe,
    GiftRoom,
    RainbowGarden,
    CloudQueen
};

struct Room {
    int x{};
    int y{};
    RoomType type{RoomType::CandyPath};
    bool visited{false};
    bool cleared{false};
};

class Dungeon {
public:
    explicit Dungeon(std::uint32_t seed = 1);

    void generate(int floorNumber);
    Room* roomAt(int x, int y);
    const Room* roomAt(int x, int y) const;
    const std::vector<Room>& rooms() const noexcept { return rooms_; }
    std::uint32_t seed() const noexcept { return seed_; }

    static std::string roomName(RoomType type);

private:
    std::uint32_t seed_;
    std::vector<Room> rooms_;
};
