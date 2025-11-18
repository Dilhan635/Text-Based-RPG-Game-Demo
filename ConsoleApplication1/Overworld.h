#pragma once
#include "NPC.h"
#include <string>
#include <vector>

// Forward declaration for LocalGrid (we'll define this later)
struct LocalGrid;

enum class TileType {
    Plains,
    Forest,
    Mountain,
    Water,
    Town,
    Dungeon,
    Landmark
};

struct OverworldTile {
    TileType type;
    bool isWalkable = true;
    bool discovered = false;
    bool isHandcrafted = false;
    std::string name;
    std::string description;
    LocalGrid* localMap = nullptr;

    OverworldTile() = default; // default construction
    OverworldTile(const OverworldTile&) = delete; // Disables copying
    OverworldTile& operator=(const OverworldTile&) = delete;
    OverworldTile(OverworldTile&&) = default; // Enables moving
    OverworldTile& operator=(OverworldTile&&) = default;
};

struct OverworldPiece {
    std::string name;
    int width;
    int height;
    std::vector<std::vector<OverworldTile>> tiles;

    OverworldPiece(int w, int h) : width(w), height(h) {
        tiles.reserve(h);
        for (int y = 0; y < h; ++y) {
            std::vector<OverworldTile> row;
            for (int x = 0; x < w; ++x) {
                row.emplace_back(); // Safe default construction
            }
            tiles.emplace_back(std::move(row));
        }
    }
    OverworldPiece(const OverworldPiece&) = delete;
    OverworldPiece& operator=(const OverworldPiece&) = delete;
    OverworldPiece(OverworldPiece&&) noexcept = default;
    OverworldPiece& operator=(OverworldPiece&&) noexcept = default;
};

using PieceGenerator = OverworldPiece(*)();

struct Overworld {
    int width;
    int height;
    std::vector<std::vector<OverworldTile>> map; // creates a grid of height and width tiles //2d vector
    std::vector<NPC> npcs; // Store procedural NPCs

    Overworld() = default; // Default constructor
    Overworld(const Overworld&) = delete; // Copy constructor
    Overworld& operator=(const Overworld&) = delete; // copy assignment operator
    Overworld(Overworld&&) = default; // move constructor
    Overworld& operator=(Overworld&&) = default; // move assignment operator
};

OverworldPiece MakeForestClearingWithDungeon();
OverworldPiece MakeProceduralTown();

Overworld GenerateOverworld(int width, int height); // Declaration

bool CanPlaceOverworldPiece(const Overworld& world, const OverworldPiece& piece, int startX, int startY);
void PlaceOverworldPiece(Overworld& world, OverworldPiece& piece, int startX, int startY);

void GrowTerrainCluster(Overworld& world, int startX, int startY, TileType type, int size);