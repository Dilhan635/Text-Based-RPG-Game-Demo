#include "NPCManager.h"
#include "Loreentry.h"


void NPCManager::AddNPC(const NPC& npc) {
    allNPCs.push_back(npc);
}

std::vector<NPC*> NPCManager::GetNPCsInArea(const std::string& areaName) {
    std::vector<NPC*> result;
    for (auto& npc : allNPCs) {
        if (npc.currentArea == areaName) {
            result.push_back(&npc);
        }
    }
    return result;
}

void NPCManager::MoveNPC(const std::string& npcName, const std::string& newArea, int newX, int newY) {
    for (auto& npc : allNPCs) {
        if (npc.name == npcName) {
            npc.currentArea = newArea;
            npc.x = newX;
            npc.y = newY;
            break;
        }
    }
}

const NPC NPCDatabase::Vaultbound = [] {
    NPC npc("Vaultbound", "A cheerful wanderer perched on a dusty pedestal.", 35, 12,
        {
            "\"Me? I'm what they call a treasure hunter.\"",
            "\"Not the flashy kind, mind you - more dust than gold these days.\"",
            "\"Damned place, for what it lacks in treasure it makes up with in tunnels.\"",
            "\"Whole network's tangled like a goblin's strategy - outer paths loop, inner halls twist... and I swear the tunnels shift when I'm not looking.\"",
            "\"Course, it ain't just us down here. Something else is sniffing around.\"",
            "\"I didn't crawl into this dump for the scenery - That beast's got something shiny I want.\"",
            "\"Tell ya what - bring me the gemstone, and I'll see about returning the favor...\""
        },
        Lore::Gemstone,
        "Vault",
        "''Well, don't let me keep you. The vault's not gonna explore itself.''",
        false
    );
    return npc;
    }();