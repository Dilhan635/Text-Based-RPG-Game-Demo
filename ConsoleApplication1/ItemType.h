#pragma once
#include <string>

enum class ItemType {
    Healing,
    Mana,
    Damage,
    Buff,
    Utility,
    MainHand,
    OffHand,
    Head,
    Torso,
    Legs,
    Accessory
};

inline std::string ItemTypeToString(ItemType type) {
    switch (type) {
    case ItemType::Healing: return "Healing";
    case ItemType::Mana: return "Mana";
    case ItemType::Damage: return "Damage";
    case ItemType::Buff: return "Buff";
    case ItemType::Utility: return "Utility";
    case ItemType::MainHand: return "Main-Hand";
    case ItemType::OffHand: return "Off-Hand";
    case ItemType::Head: return "Head";
    case ItemType::Torso: return "Torso";
    case ItemType::Legs: return "Legs";
    case ItemType::Accessory: return "Accessory";
    default: return "Unknown";
    }
}
