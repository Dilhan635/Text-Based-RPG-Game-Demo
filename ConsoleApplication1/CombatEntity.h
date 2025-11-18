#pragma once
#include <string>
#include <vector>
#include "StatusEffect.h"

class CombatEntity {
public:
    std::string name;
    int currentHealth;
    int maxHealth;
    int agility;
    bool skipTurn = false;

    std::vector<StatusEffect> activeEffects;

    // Optional: base stats for modifiers
    int baseMaxHealth = 0;
    int baseAgility = 0;

    virtual ~CombatEntity() = default; // /use the default constructor
};

