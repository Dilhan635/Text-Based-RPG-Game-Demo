#pragma once
#include <string>

enum class SpellType {
    Damage,
    Healing,
    Cleansing,
    Buff,
    Debuff,
    Utility
};

enum class SpellCastResult {
    Success,
    Failed,
    NeedsCombatDamage
};

class Spell {
public:
    std::string name;
    std::string description;
    SpellType type;
    int manaCost;
    int value; //potency
    bool usableInCombat;
    bool usableOutsideCombat;

    Spell(const std::string& n, // Constructor
        const std::string& desc,
        SpellType t,
        int cost, int val,
        bool inCombat,
        bool outCombat)
        : name(n),
        description(desc),
        type(t),
        manaCost(cost),
        value(val),
        usableInCombat(inCombat),
        usableOutsideCombat(outCombat) {
    }

    Spell() = default;
};