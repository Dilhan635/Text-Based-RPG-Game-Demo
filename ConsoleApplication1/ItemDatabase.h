#pragma once
#include "Item.h"
#include "ItemData.h"
#include <unordered_map>

namespace ItemDatabase {
    extern const ItemData FadedLetter;
    extern const ItemData HealingSalve;
    extern const ItemData ManaTonic;
    extern const ItemData RustySword;
    extern const ItemData WornArmour;
    extern const ItemData OldBow;
    extern const ItemData BeastsGemstone;

    // Add more declarations

    const std::unordered_map<std::string, const ItemData*> itemMap = {
        {"FadedLetter", &FadedLetter},
        {"HealingSalve", &HealingSalve},
        {"ManaTonic", &ManaTonic},
        {"RustySword", &RustySword},
        {"WornArmour", &WornArmour},
        {"OldBow", &OldBow}
    };
}
