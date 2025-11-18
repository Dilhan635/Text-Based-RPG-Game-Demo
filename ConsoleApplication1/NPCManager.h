#pragma once
#include <string>
#include <vector>
#include "NPC.h"

class NPCManager {
public:
    std::vector<NPC> allNPCs;

    // Add a new NPC to the manager
    void AddNPC(const NPC& npc);

    // Get all NPCs currently in a specific area
    std::vector<NPC*> GetNPCsInArea(const std::string& areaName);

    // Move an NPC to a new area and position
    void MoveNPC(const std::string& npcName, const std::string& newArea, int newX, int newY);

    // named npc

};

class NPCDatabase {
public:
    static const NPC Vaultbound;
    // Add more NPCs here
};
