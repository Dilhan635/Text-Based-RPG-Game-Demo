#include "EnemyPool.h"
#include "ItemDatabase.h"

std::vector<Enemy> GenerateForestEnemies() {
    return {
        Enemy("Slime", 3, 2, 1, 1, 0, 0, "Monster", "Growth", false, 30, 15,
              GetRandomPersonality(), { LootDrop{ &ItemDatabase::HealingSalve, 0.5f}}),
        Enemy("Bandit", 5, 3, 3, 2, 0, 0, "Human", "Slash", false, 45, 30,
              GetRandomPersonality(), { LootDrop{ &ItemDatabase::HealingSalve, 0.5f}})
    };
}

std::vector<Enemy> GenerateDungeonEnemies() {
    return {
        Enemy("Skeletal Warrior", 4, 3, 2, 1, 0, 0, "Undead", "Lunge", false, 40, 25,
              GetRandomPersonality(), { LootDrop{ &ItemDatabase::HealingSalve, 0.5f}}),
        Enemy("Skeletal Mage", 4, 2, 1, 5, 0, 0, "Magic", "Burn", false, 50, 10,
              GetRandomPersonality(), { LootDrop{ &ItemDatabase::ManaTonic, 0.5f}})
    };
}

std::vector<Enemy> GenerateBossEnemies() {
    return {
        Enemy("Beast of the Goblin Fortress", 8, 6, 3, 3, 2, 3, "Monster", "Rampage", true, 120, 80,
              GetRandomPersonality(), { LootDrop{ &ItemDatabase::BeastsGemstone, 1.0f}})
    };
}
