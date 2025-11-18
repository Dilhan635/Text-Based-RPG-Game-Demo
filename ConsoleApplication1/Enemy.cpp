#include "Enemy.h"
#include <cstdlib> // for rand()

// member initializer list
Enemy::Enemy(std::string n, int vit, int str, int agi, int intl, int posX, int posY,
    std::string t, std::string ab, bool boss,
    int exp, int gold,
    PersonalityType p,
    std::vector<LootDrop> loot)
    : vitality(vit), strength(str), intelligence(intl),
    type(t), ability(ab), isBoss(boss),
    expReward(exp), goldReward(gold),
    x(posX), y(posY), personality(p), lootTable(loot) {

    // has to be initialised here in the constructor body since they are inherited
    name = n;
    baseMaxHealth = vit * 10;
    maxHealth = baseMaxHealth;
    currentHealth = maxHealth;
    baseAgility = agi;
    agility = baseAgility;
}

PersonalityType GetRandomPersonality() {
    int roll = rand() % 5;
    switch (roll) {
    case 0: return PersonalityType::TIMID;
    case 1: return PersonalityType::AGGRESSIVE;
    case 2: return PersonalityType::STALWART;
    case 3: return PersonalityType::CUNNING;
    case 4: return PersonalityType::FANATICAL;
    default: return PersonalityType::TIMID;
    }
}

std::vector<const ItemData*> Enemy::RollLoot() const {
    std::vector<const ItemData*> dropped;
    for (const auto& entry : lootTable) {
        float roll = static_cast<float>(rand()) / RAND_MAX;
        if (roll <= entry.dropChance) {
            dropped.push_back(entry.item);
        }
    }
    return dropped;
}
