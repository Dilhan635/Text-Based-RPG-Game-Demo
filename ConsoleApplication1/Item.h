#pragma once
#include <string>
#include "ItemData.h"

// Forward declarations
class Player;
class Enemy;

class Item {
public:
    ItemData data;

    Item() = default;
    Item(const ItemData& itemData); // Stored pointer to shared ItemData

    //void UseInCombat(Player& player, Enemy& enemy);
    //void Inspect() const;
    // Add merchant/loot logic later
};

