#include "ItemDatabase.h"

const ItemData ItemDatabase::FadedLetter = {
    "Faded Letter",
    "A tattered letter found deep within your pocket.The parchment is torn, the ink faded and smudged.Most of the message is lost, but one line remains legible : \n\n"
        "\"If Her Majesty's safety means anything to you, meet me inside the bell tower. Do not keep me waiting.\"\n\n"
        "-G.",
    ItemType::Utility,
    0,
    false,
    false,
    false,
    1,
    "Someone wishes to meet you at the Bell Tower",
    false,
    0,
    0,
    0,
    0
};

const ItemData ItemDatabase::HealingSalve = {
    "Healing Salve",
    "A pungent blend of crushed herbs that restores 25 HP.",
    ItemType::Healing,
    30,
    true,
    true,
    true,
    1,
    "",
    false,
    0,
    0,
    0,
    0
};

const ItemData ItemDatabase::ManaTonic = {
    "Mana Tonic",
    "An azure concoction of infused herbs that restores 25 MP",
    ItemType::Mana,
    30,
    true,
    true,
    true,
    1,
    "",
    false,
    0,
    0,
    0,
    0

};

const ItemData ItemDatabase::RustySword = {
    "Rusty Sword",
    "A weathered sword lost to time...",
    ItemType::MainHand,     // Slot: Main Hand
    0,                      // Value
    false,                  // Consumable
    false,                  // Usable in combat
    false,                  // Usable in interaction
    1,                      // Quantity
    "",                     // Lore entry
    true,                   // Equippable
    2,                      // Strength bonus
    0,                      // Vitality bonus
    0,                      // Agility bonus
    0                       // Intelligence bonus
};

const ItemData ItemDatabase::WornArmour = {
    "Worn Armour",
    "",
    ItemType::Torso,
    0,
    false,
    false,
    false,
    1,
    "",
    true,
    0,
    2,
    0,
    0
};

const ItemData ItemDatabase::OldBow = {
    "Old Bow",
    "",
    ItemType::MainHand,
    0,
    false,
    false,
    false,
    1,
    "",
    true,
    1,
    0,
    1,
    0
};

const ItemData ItemDatabase::BeastsGemstone = {
    "Beast's Gemstone",
    "",
    ItemType::Utility,
    0,
    false,
    false,
    false,
    1,
    "",
    false,
    0,
    0,
    0,
    0
};