#include "LocalGrid.h"
#include "LocalGridGenerator.h"
#include "EnemyPool.h"
#include "NPC.h"
#include "NPCManager.h"
#include <vector>
#include <string>
#include <functional>
#include <cstdlib> // for rand()
#include <iostream>

// Room helpers
void AddMiniRoom(LocalGrid* grid, int offsetX, int offsetY, const std::string& description) {
    for (int y = 0; y <= 2; ++y) {
        for (int x = 0; x <= 4; ++x) {
            int gx = offsetX + x;
            int gy = offsetY + y;
            if (gy >= 0 && gy < grid->height && gx >= 0 && gx < grid->width) {
                grid->tiles[gy][gx].type = LocalTileType::Floor;
                grid->tiles[gy][gx].walkable = true;
                grid->tiles[gy][gx].description = description;
            }
        }
    }
}

void AddSmallRoom(LocalGrid* grid, int offsetX, int offsetY, const std::string& description) {
    for (int y = 0; y <= 4; ++y) {
        for (int x = 0; x <= 4; ++x) {
            int gx = offsetX + x;
            int gy = offsetY + y;
            if (gy >= 0 && gy < grid->height && gx >= 0 && gx < grid->width) {
                grid->tiles[gy][gx].type = LocalTileType::Floor;
                grid->tiles[gy][gx].walkable = true;
                grid->tiles[gy][gx].description = description;
            }
        }
    }
}

void AddLargeRoom(LocalGrid* grid, int offsetX, int offsetY, const std::string& description) {
    for (int y = 0; y <= 8; ++y) {
        for (int x = 0; x <= 8; ++x) {
            int gx = offsetX + x;
            int gy = offsetY + y;
            if (gy >= 0 && gy < grid->height && gx >= 0 && gx < grid->width) {
                grid->tiles[gy][gx].type = LocalTileType::Floor;
                grid->tiles[gy][gx].walkable = true;
                grid->tiles[gy][gx].description = description;
            }
        }
    }
}

void AddCorridorVertical(LocalGrid* grid, int offsetX, int offsetY, const std::string& description) {
    for (int y = 0; y < 5; ++y) {
        int gx = offsetX;
        int gy = offsetY + y;
        if (gy >= 0 && gy < grid->height && gx >= 0 && gx < grid->width) {
            grid->tiles[gy][gx].type = LocalTileType::Floor;
            grid->tiles[gy][gx].walkable = true;
            grid->tiles[gy][gx].description = description;
        }
    }
}

void AddCorridorHorizontal(LocalGrid* grid, int offsetX, int offsetY, const std::string& description) {
    for (int x = 0; x < 5; ++x) {
        int gx = offsetX + x;
        int gy = offsetY;
        if (gy >= 0 && gy < grid->height && gx >= 0 < grid->width) {
            grid->tiles[gy][gx].type = LocalTileType::Floor;
            grid->tiles[gy][gx].walkable = true;
            grid->tiles[gy][gx].description = description;
        }
        
    }
}

void AddCrossIntersection(LocalGrid* grid, int offsetX, int offsetY, const std::string& description) {
    // Vertical corridor (5 tiles)
    for (int y = -2; y <= 2; ++y) {
        int gx = offsetX;
        int gy = offsetY + y;
        if (gy >= 0 && gy < grid->height && gx >= 0 && gx < grid->width) {
            grid->tiles[gy][gx].type = LocalTileType::Floor;
            grid->tiles[gy][gx].walkable = true;
            grid->tiles[gy][gx].description = description;
        }
    }

    // Horizontal corridor (5 tiles)
    for (int x = -2; x <= 2; ++x) {
        int gx = offsetX + x;
        int gy = offsetY;
        if (gy >= 0 && gy < grid->height && gx >= 0 && gx < grid->width) {
            grid->tiles[gy][gx].type = LocalTileType::Floor;
            grid->tiles[gy][gx].walkable = true;
            grid->tiles[gy][gx].description = description;
        }
    }
}

LocalGrid* GenerateSimpleDungeon(int width, int height) {

    LocalGrid* grid = new LocalGrid();
    grid->width = width;
    grid->height = height;
    grid->tiles.resize(height);
    for (int y = 0; y < height; ++y) {
        grid->tiles[y].resize(width);
        for (int x = 0; x < width; ++x) {
            grid->tiles[y][x] = LocalTile{};
        }
    }

    for (int y = 0; y < grid->height; ++y) {
        if (grid->tiles[y].size() != grid->width) {
            std::cerr << "Row " << y << " has " << grid->tiles[y].size()
                << " tiles, expected " << grid->width << "\n";
        }
    }

    // Fill with default walls
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid->tiles[y][x].type = LocalTileType::Wall;
            grid->tiles[y][x].walkable = false;
            grid->tiles[y][x].description = "Solid stone blocks forming the dungeon's structure.";
        }
    }

    // ROOM PARTS PLACEMENT

    // Starting Cell - Detention Block
    AddSmallRoom(grid, 2, 2, "The cell is vast and silent, its rusted chains still grasping the bones of long-dead captives. A breach in the southern wall exposes a passage to the darkness beyond. \n");

    // Western Outer Path
    AddCorridorVertical(grid, 4, 7, "The tunnel continues on through the abyss.\n");
    AddCorridorVertical(grid, 4, 15, "The tunnel continues on through the abyss.\n");
    AddCorridorVertical(grid, 4, 20, "The tunnel continues on through the abyss.\n");
    AddCorridorVertical(grid, 4, 30, "\n");

    // Western Outer Mini Room - Guard's Quarters
    AddMiniRoom(grid, 2, 12, "A bunk lies overturned, its sheets clawed. Dust and debris blanket the floor, and the air carries the stale scent of sweat and iron.\n");
    // thinking of an item to place here that makes sense

    // Western Outer Small Room - Supply Depot
    AddSmallRoom(grid, 2, 25, "A collapsed chamber with thick stone walls and splintered storage crates. The remnants of rations and broken tools lie scattered across the floor, long since looted.\n"); 

    // Southern Inner Path
    AddCorridorHorizontal(grid, 7, 26, "\n");
    AddCorridorHorizontal(grid, 17, 24, "\n");
    AddCorridorVertical(grid, 14, 19, "The tunnel continues on through the abyss.\n");

    // Southern Inner Mini Room - Food Hall
    AddMiniRoom(grid, 12, 24, "Long stone tables sit overturned, and broken ceramic bowls crunch underfoot. A faint aroma of dried herbs lingers, oddly preserved in the stagnant air.\n");
    PlaceItem(grid, 13, 25, "HealingSalve");

    // Southern Outer Path
    AddCorridorHorizontal(grid, 4, 35, "\n");
    AddCorridorHorizontal(grid, 9, 35, "\n");
    AddCorridorHorizontal(grid, 14, 35, "\n");
    AddCorridorHorizontal(grid, 24, 35, "\n");
    AddCorridorHorizontal(grid, 29, 35, "\n");

    // Southern Outer Room - Barracks
    AddSmallRoom(grid, 19, 33, "Weapon racks stand empty, and faded banners hang limp from the walls. The room feels heavy with the memory of drills and discipline now lost to time.\n");
    PlaceItem(grid, 19, 33, "WornArmour");

    // Eastern Inner Path
    AddCorridorVertical(grid, 23, 28, "\n");
    AddCorridorVertical(grid, 24, 18, "The tunnel continues on through the abyss.\n");
    AddCorridorVertical(grid, 24, 10, "The tunnel continues on through the abyss.\n");
    AddCorridorHorizontal(grid, 19, 10, "\n");

    // Eastern Inner Mini Room - Armoury Niche
    AddMiniRoom(grid, 22, 15, "Rust-streaked blades and dented shields line the walls. The chamber is narrow and cold, its silence broken only by the occasional drip of water from above.\n");
    PlaceItem(grid, 23, 15, "OldBow");

    // Southern Inner Small Room - Ritual Sanctum
    AddSmallRoom(grid, 22, 23, "Arcane symbols are etched into the stone floor, glowing faintly with residual energy. A scorched pedestal stands at the center, surrounded by crumbled offerings and broken runes.\n");
    PlaceSpell(grid, 22, 24, "Kindle");

    // Eastern Outer Path
    AddCorridorVertical(grid, 33, 15, "\n");
    AddCorridorVertical(grid, 33, 20, "\n");
    AddCorridorVertical(grid, 33, 25, "\n");
    AddCorridorVertical(grid, 33, 30, "\n");

    // Eastern Outer Mini Room - Vault
    AddMiniRoom(grid, 31, 12, "Thick stone walls enclose the chamber, and the air is unnaturally still. Rows of empty pedestals line the room, their surfaces worn smooth by time and reverence. Faint impressions in the dust suggest that something valuable once rested here — or perhaps still does, hidden from plain sight.\n");
    PlaceGold(grid, 35, 12, 25);
    PlaceNPC(grid, 31, 13, NPCDatabase::Vaultbound);

    // Grand Hall
    AddLargeRoom(grid, 10, 10, "A spacious room unfolds in the dark.The silence hangs heavy, pressing in from every side. Cracked tiles and faded murals hint at a time when this hall echoed with command and ceremony.\n"); //central zone (sword found here)
    PlaceItem(grid, 15, 15, "RustySword");

    // Northern Inner Path
    AddCorridorVertical(grid, 14, 5, "\n");

    // Boss Room - Commander's Watch
    AddMiniRoom(grid, 12, 2, "This high chamber overlooks the northern corridors. Maps and tactical plans lie scattered across a stone table, and a broken helm rests on a throne-like chair. The air is tense, as if the commander's presence still lingers.\n");

    // Northern Outer Path
    AddCorridorHorizontal(grid, 17, 3, "\n");
    AddCorridorHorizontal(grid, 22, 3, "\n");
    AddCorridorHorizontal(grid, 31, 1, "\n");

    // Gate
    AddMiniRoom(grid, 26, 1, "A massive iron gate looms at the far end of the room, rusted but still imposing. Wind whistles through the cracks, carrying whispers from the world beyond. The path ahead feels final.\n");

    grid->enemyPool = GenerateDungeonEnemies(); // or GenerateDungeonEnemies() based on location

    // Boss
    Enemy boss = GenerateBossEnemies()[0]; //first boss in index
    PlaceBoss(grid, 14, 9, boss);

    grid->tiles[5][5].type = LocalTileType::Exit; // 1 35
    grid->tiles[5][5].walkable = true;

    return grid;
}

void PlaceItem(LocalGrid* grid, int x, int y, const std::string& name) {
    auto& tile = grid->tiles[y][x];
    tile.type = LocalTileType::Item;
    tile.walkable = true;
    //tile.description = description;
    tile.itemName = name;
}

void PlaceSpell(LocalGrid* grid, int x, int y, const std::string& name) {
    auto& tile = grid->tiles[y][x];
    tile.type = LocalTileType::Item;
    tile.walkable = true;
    tile.itemName = name;
}

void PlaceGold(LocalGrid* grid, int x, int y, int gold) {
    auto& tile = grid->tiles[y][x];
    tile.type = LocalTileType::Gold;
    tile.walkable = true;
    tile.goldAmount = gold;
}

void PlaceNPC(LocalGrid* grid, int x, int y, const NPC& npc) {
    auto& tile = grid->tiles[y][x];
    tile.type = LocalTileType::NPC;
    tile.walkable = true;
    tile.hasNPC = true;
    tile.npc = npc;
}

void PlaceBoss(LocalGrid* grid, int x, int y, const Enemy& boss) {
    auto& tile = grid->tiles[y][x];
    tile.type = LocalTileType::Boss;
    tile.walkable = true;
    tile.enemy = std::make_unique<Enemy>(boss); // creates a new enemy
}