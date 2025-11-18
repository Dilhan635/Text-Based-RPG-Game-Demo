#pragma once
#include <string>
#include <vector>

enum class StatusType {
    Poisoned,
    Burned,
    Frostbitten,
    Stunned,
    Confused,
    Anxious
};

struct StatusEffect {
    StatusType type;
    int duration;
    int damagePerTurn = 0;
    float maxHPModifier = 1.0f;
    float strengthModifier = 1.0f;
    float agilityModifier = 1.0f;
    float accuracyModifier = 1.0f;
    float critModifier = 1.0f;
    float escapePenalty = 0.0f;
    bool skipTurn = false;
    float skipChance = 0.0f;
    float randomActionChance = 0.0f;
};

class CombatEntity;

void ApplyStatusEffects(std::vector<StatusEffect>& effects, CombatEntity& target);
