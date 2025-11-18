#pragma once
#include "Spell.h"
#include <unordered_map>

namespace SpellDatabase {
    extern const Spell Kindle; //fire tier 1
    extern const Spell Chill; //ice tier 1
    extern const Spell Jolt; //electric tier 1
    extern const Spell Heal; //heal tier 1
    extern const Spell Cleanse; //status cleanse tier 1
    extern const Spell Scorch; //fire tier 2
    extern const Spell Freeze; //ice tier 2
    extern const Spell Electrocute; //electric tier 2
    extern const Spell Fracture; //earth tier 2
    extern const Spell Toxic; //poison tier 2
    extern const Spell Cure; //heal tier 2
    extern const Spell Inferno; //fire tier 3
    extern const Spell Blizzard; //ice tier 3
    extern const Spell Thunderstorm; //electric tier 3
    extern const Spell Earthquake; //earth tier 3
    extern const Spell Plague; //poison tier 3
    extern const Spell Life; //heal tier 3

    const std::unordered_map<std::string, const Spell*> spellMap = {
        {"Kindle", &Kindle},
        {"Chill", &Chill},
        {"Jolt", &Jolt},
        {"Heal", &Heal},
        {"Cleanse", &Cleanse}
    };
}
