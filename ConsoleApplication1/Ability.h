#pragma once
#include <string>

class Ability {
public:
    std::string name;             // Format: "EnemyName AbilityName"
    std::string description;      // Includes damage, defense, or effect details
    bool usableInCombat;         // True if usable during battle
    bool usableOutsideCombat;    // True if usable in overworld

    Ability(const std::string& n,
        const std::string& desc,
        bool inCombat,
        bool outCombat)
        : name(n),
        description(desc),
        usableInCombat(inCombat),
        usableOutsideCombat(outCombat) {
    }
};

