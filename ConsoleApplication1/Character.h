#pragma once
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "Spell.h"
#include "Ability.h"
#include "Item.h"
#include "Loreentry.h"
#include "CombatEntity.h"

struct Stat {
	int value = 1;
	int xp = 0;
	int xpToNextLvl = 60;

	// Constructor that scales XP threshold based on starting value
	Stat(int startingValue = 1) : value(startingValue) {
		xpToNextLvl = 30 + (value - 1) * 15; // Example: 80 base + 25 per stat level
	}

	void GainXP(int amount) {
		xp += amount;
		while (xp >= xpToNextLvl) {
			xp -= xpToNextLvl;
			value++;
			xpToNextLvl = static_cast<int>(xpToNextLvl * 1.20); //scales up
			std::cout << "Stat increased to:" << value << "!\n";

		}
	}


};

struct Character : public CombatEntity {
	//std::string name;
	std::string race;

	Stat vitality; //health and defense
	Stat strength; //melee damage, carrying capacity
	Stat agility; //movement speed, dodge chance
	Stat intelligence; //mana, magic damage, better dialogue

	//out of combat skills
	Stat crafting;
	Stat fishing;

	int GetHealthRecoveryPerTurn() const;
	int GetManaRecoveryPerTurn() const;

	void RecoverHealthAndManaPerTurn();

	int currentMana;
	int gold = 0;

	std::vector<Spell> spells;
	std::vector<Ability> abilities;
	std::vector<Item> inventory;
	std::vector<LoreEntry> loreEntries;

	std::map<ItemType, size_t> equipmentIndex;


	Character() { // Default Constructor
		InitializeStats();
	}

	Character(const std::string& name, const std::string& race)
		: race(race) {
		this->name = name;
		InitializeStats();
	}

	int GetMaxHealth() const;
	void HealToFull();
	int GetMaxMana() const;
	void ReplenishManaToFull();

	void InitializeStats();
	void AddGold(int amount);
	void AddItem(const Item& newItem);

	bool EquipItem(size_t index);
	std::string GetSlotName(ItemType type) const;
	bool UnequipItem(ItemType slot);

	int GetTotalStrength() const;
	int GetTotalVitality() const;
	int GetTotalAgility() const;
	int GetTotalIntelligence() const;

	void LearnSpell(const Spell& spell);
	void LearnAbility(const Ability& ability);
	bool HasDiscoveredLore(const std::string& content) const;
	void DiscoverLore(const LoreEntry& lore);

	void ViewAbilities() const;
	void ViewItems(bool allowUse = false, const std::string& context = "menu");
	void ViewEquipment() const;
	void ViewLore() const;

	SpellCastResult ViewSpells(bool allowUse = false, const std::string& context = "menu");
	Spell lastCastSpell;
	void SetLastCastSpell(const Spell& spell);
	bool hasCastSpell = false;
	const Spell* GetLastCastSpell() const;
	SpellCastResult HandleSpellCast(const Spell& spell, const std::string& context);

	std::vector<size_t> RefreshVisibleIndices(const std::string& context);
	void DisplayInventory(const std::vector<size_t>& indices);
	void InspectItem(const Item& item);
	void InspectSpell(const Spell& spell);
	bool HandleItemAction(Item& item, bool isEquipped,int action, const std::string& context, size_t index);

};

Character CreateCharacter();

