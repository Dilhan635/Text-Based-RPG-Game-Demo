#include "SpellDatabase.h"

const Spell SpellDatabase::Kindle = {
    "Kindle", // name
    "Ignites a small flame to singe a single target.", // description
    SpellType::Damage, // spell type
    10, // mana cost
    5, // potency value
    true, // usable in combat
    false // usable out of combat
};

const Spell SpellDatabase::Chill = {
    "Chill",
    "Slows the target with a burst of cold.",
    SpellType::Damage,
    5,
    5,
    true,
    false
};

const Spell SpellDatabase::Jolt = {
    "Jolt",
    "Delivers a quick electric shock.",
    SpellType::Damage,
    5,
    5,
    true,
    false
};

const Spell SpellDatabase::Heal = {
    "Heal",
    "Restores a small amount of health.",
    SpellType::Damage,
    6,
    5,
    true,
    true
};

const Spell SpellDatabase::Cleanse = {
    "Cleanse",
    "Removes a minor status effect.",
    SpellType::Damage,
    4,
    5,
    false,
    true
};

const Spell SpellDatabase::Scorch = {
    "Scorch",
    "Burns the target with intense heat.",
    SpellType::Damage,
    10,
    5,
    true,
    false
};

const Spell SpellDatabase::Freeze = {
    "Freeze",
    "Freezes the target in place.",
    SpellType::Damage,
    10,
    5,
    true,
    false
};

const Spell SpellDatabase::Electrocute = {
    "Electrocute",
    "Strikes with a powerful electric blast.",
    SpellType::Damage,
    10,
    5,
    true,
    false
};

const Spell SpellDatabase::Fracture = {
    "Fracture",
    "Cracks the ground beneath the enemy.",
    SpellType::Damage,
    10,
    5,
    true,
    false
};

const Spell SpellDatabase::Toxic = {
    "Toxic",
    "Poisons the target over time.",
    SpellType::Damage,
    10,
    5,
    true,
    false
};

const Spell SpellDatabase::Cure = {
    "Cure",
    "Heals moderate HP and removes basic ailments.",
    SpellType::Damage,
    10,
    5,
    true,
    true
};

const Spell SpellDatabase::Inferno = {
    "Inferno",
    "Engulfs enemies in a wave of fire.",
    SpellType::Damage,
    18,
    5,
    true,
    false
};

const Spell SpellDatabase::Blizzard = {
    "Blizzard",
    "Summons a storm of ice and snow.",
    SpellType::Damage,
    18,
    5,
    true,
    false
};

const Spell SpellDatabase::Thunderstorm = {
    "Thunderstorm",
    "Calls down lightning on multiple foes.",
    SpellType::Damage,
    18,
    5,
    true,
    false
};

const Spell SpellDatabase::Earthquake = {
    "Earthquake",
    "Shakes the battlefield violently.",
    SpellType::Damage,
    18,
    5,
    true,
    false
};

const Spell SpellDatabase::Plague = {
    "Plague",
    "Spreads virulent poison to all enemies.",
    SpellType::Damage,
    18,
    5,
    true,
    false
};

const Spell SpellDatabase::Life = {
    "Life",
    "Fully restores HP and cures all conditions.",
    SpellType::Damage,
    20,
    5,
    true,
    true
};
