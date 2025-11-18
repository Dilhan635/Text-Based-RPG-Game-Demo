#pragma once
#include <string>
#include <vector>
#include "ItemData.h"
#include "CombatEntity.h"

enum class PersonalityType {
	TIMID,
	AGGRESSIVE,
	STALWART,
	CUNNING,
	FANATICAL

};

PersonalityType GetRandomPersonality();

struct LootDrop {
	const ItemData* item;
	float dropChance;
};

struct Enemy : CombatEntity {

	int vitality;
	int strength;
	int intelligence;
	std::string type; // enemy type
	std::string ability; //enemy abilities
	bool isBoss = false;

	int expReward = 0;
	int goldReward = 0;

	int x;
	int y;

	PersonalityType personality;

	std::vector<LootDrop> lootTable;
	std::vector<const ItemData*> RollLoot() const;

	//Constructor
	Enemy(std::string n, int vit, int str, int agi, int intl, int posX, int posY,
		std::string t = "unknown", std::string ab = "", bool boss = false,
		int exp = 0, int gold = 0,
		PersonalityType p = PersonalityType::TIMID,
		std::vector<LootDrop> loot = {});

};

