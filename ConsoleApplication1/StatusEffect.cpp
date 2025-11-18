#include "StatusEffect.h"
#include "CombatEntity.h"
#include <algorithm>
#include <iostream>
#include <cstdlib> //for rand()

void ApplyStatusEffects(std::vector<StatusEffect>& effects, CombatEntity& target) {
    for (auto& effect : effects) {
        switch (effect.type) {
        case StatusType::Poisoned:

            break;
        case StatusType::Burned:

            break;
        case StatusType::Frostbitten:
            break;
        case StatusType::Stunned:

            break;
        case StatusType::Confused:

            break;
        case StatusType::Anxious:

            break;
        }
        effect.duration--;
    }

    // Remove expired effects
    effects.erase(
        std::remove_if(effects.begin(), effects.end(),
            [](const StatusEffect& e) { return e.duration <= 0; }),
        effects.end()
    );
}
