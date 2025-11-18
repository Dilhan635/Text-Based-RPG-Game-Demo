#pragma once
#include <string>
#include "ItemType.h"

struct ItemData {
    std::string name;
    std::string description;
    ItemType type;
    int value;
    bool consumable;
    bool usableInCombat;
    bool usableOutsideCombat;
    int quantity;
    std::string loreEntry;
    bool equippable = false;

    int strengthBonus = 0;
    int vitalityBonus = 0;
    int agilityBonus = 0;
    int intelligenceBonus = 0;

};
