#include "Game.h"

#include <algorithm>
#include <iostream>

Game::Game(std::uint32_t seed) : seed_(seed), dungeon_(seed) {
    newRun();
}

void Game::newRun() {
    eva_ = EvaState{};
    floor_ = 1;
    x_ = 0;
    y_ = 0;
    running_ = true;
    dungeon_ = Dungeon(seed_);
    dungeon_.generate(floor_);
    std::cout << "\nДобро пожаловать, Ева Балабанова, в Розовое Королевство!\n";
    printStatus();
}

bool Game::move(int dx, int dy) {
    Room* target = dungeon_.roomAt(x_ + dx, y_ + dy);
    if (target == nullptr) {
        std::cout << "Там только пушистые розовые облака. Дороги нет.\n";
        return false;
    }
    x_ += dx;
    y_ += dy;
    enterRoom(*target);
    return true;
}

void Game::enterRoom(Room& room) {
    room.visited = true;
    std::cout << "\nЕва входит: " << Dungeon::roomName(room.type) << "\n";
    if (!room.cleared) resolveRoom(room);
    printStatus();
}

void Game::resolveRoom(Room& room) {
    switch (room.type) {
        case RoomType::Start:
            break;
        case RoomType::CandyPath:
            std::cout << "Озорная зефирка прячет звёздочку. Нажми x, чтобы применить магию.\n";
            break;
        case RoomType::CatCafe:
            eva_.health = eva_.maxHealth;
            ++eva_.cupcakes;
            room.cleared = true;
            std::cout << "Котята угостили Еву кексом. Здоровье восстановлено!\n";
            break;
        case RoomType::GiftRoom:
            ++eva_.magic;
            eva_.stars += 2;
            room.cleared = true;
            std::cout << "В подарке новая сила палочки и две звёздочки!\n";
            break;
        case RoomType::RainbowGarden:
            std::cout << "Радужная слизняшка хочет поиграть. Используй магию.\n";
            break;
        case RoomType::CloudQueen:
            std::cout << "Облачная Королева ждёт дружеского магического поединка!\n";
            break;
    }
}

void Game::useMagic() {
    Room* room = dungeon_.roomAt(x_, y_);
    if (room == nullptr || room->cleared) {
        std::cout << "Здесь уже всё спокойно и мило.\n";
        return;
    }

    const int reward = (room->type == RoomType::CloudQueen) ? 5 : 1;
    eva_.stars += reward;
    room->cleared = true;
    std::cout << "Ева выпускает сияющее сердечко! Получено звёздочек: " << reward << ".\n";

    if (room->type == RoomType::CloudQueen) nextFloor();
}

void Game::eatCupcake() {
    if (eva_.cupcakes <= 0) {
        std::cout << "Кексы закончились. Нужно найти Кошачье кафе.\n";
        return;
    }
    --eva_.cupcakes;
    eva_.health = std::min(eva_.maxHealth, eva_.health + 4);
    std::cout << "Ева съела розовый кекс и восстановила силы.\n";
    printStatus();
}

void Game::nextFloor() {
    ++floor_;
    x_ = 0;
    y_ = 0;
    dungeon_.generate(floor_);
    std::cout << "\nОстров спасён! Открывается волшебный портал на этаж " << floor_ << ".\n";
}

void Game::printStatus() const {
    std::cout << "ЕВА | этаж: " << floor_
              << " | сердечки: " << eva_.health << '/' << eva_.maxHealth
              << " | звёздочки: " << eva_.stars
              << " | кексы: " << eva_.cupcakes
              << " | сила палочки: " << eva_.magic << "\n";
}
