#pragma once   
#include "Enemy.h"
#include "NPC.h"
#include <string>
#include <vector>
#include <memory> // for smart pointer

// Optional: define types for local tiles
enum class LocalTileType {
    Empty,
    Wall,
    Floor,
    Door,
    NPC,
    Item,
    Gold,
    Boss,
    Exit
};

struct LocalTile {
    LocalTileType type;
    bool walkable = true;
    std::string description;

    // Item and Gold
    std::string itemName; //empty if no item
    int goldAmount = 0; // no gold if 0

    // NPC support
    bool hasNPC = false;
    NPC npc; // or NPC* if prefer dynamic allocation

    // Boss
    std::unique_ptr<Enemy> enemy; // smart pointer

    LocalTile() = default;
    LocalTile(const LocalTile&) = default;
    LocalTile& operator=(const LocalTile&) = default;
    LocalTile(LocalTile&&) = default;
    LocalTile& operator=(LocalTile&&) = default;
};

struct LocalGrid {
    int width;
    int height;
    std::vector<std::vector<LocalTile>> tiles;
    std::vector<Enemy> enemyPool;

    LocalGrid() = default;
    LocalGrid(const LocalGrid&) = default;
    LocalGrid& operator=(const LocalGrid&) = default;
    LocalGrid(LocalGrid&&) = default;
    LocalGrid& operator=(LocalGrid&&) = default;
};

