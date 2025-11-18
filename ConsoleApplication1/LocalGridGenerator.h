#pragma once
#include <functional>
#include <string>
#include <vector>
#include "LocalGrid.h"

struct RoomTemplate {
    std::string name;
    int width;
    int height;
    std::function<void(LocalGrid*, int, int)> builder;
};

void AddMiniRoom(LocalGrid* grid, int offsetX, int offsetY, const std::string& description);
void AddSmallRoom(LocalGrid* grid, int offsetX, int offsetY, const std::string& description);
void AddLargeRoom(LocalGrid* grid, int offsetX, int offsetY, const std::string& description);
void AddCorridorVertical(LocalGrid* grid, int offsetX, int offsetY, const std::string& description);
void AddCorridorHorizontal(LocalGrid* grid, int offsetX, int offsetY, const std::string& description);
void AddCrossIntersection(LocalGrid* grid, int offsetX, int offsetY, const std::string& description);

void PlaceGold(LocalGrid* grid, int x, int y, int gold);
void PlaceItem(LocalGrid* grid, int x, int y, const std::string& name);
void PlaceSpell(LocalGrid* grid, int x, int y, const std::string& name);
void PlaceNPC(LocalGrid* grid, int x, int y, const NPC& npc);
void PlaceBoss(LocalGrid* grid, int x, int y, const Enemy& boss);

LocalGrid* GenerateSimpleDungeon(int width, int height);
