#include "Character.h"
#include "Spell.h"
#include "Ability.h"
#include "Item.h"
#include "Loreentry.h"
#include <iostream>


Character CreateCharacter() {
    std::string name;
    std::string race;
    std::string choice;

	std::cout << "Enter your character's name: ";
	std::getline(std::cin, name);

	std::cout << "Select a race (Human, Orc, Elf): ";
	std::getline(std::cin, choice);

    if (choice == "Human" || choice == "human") {
        race = "Human";
    }
    else if (choice == "Orc" || choice == "orc") {
        race = "Orc";
    }
    else if (choice == "Elf" || choice == "elf") {
        race = "Elf";
    }
    else {
        race = "Human"; // fallback/default
        std::cout << "Invalid choice. Defaulting to Human.\n";
    }

    Character player(name, race);

    //stats based on race
	if (race == "Human") {
		player.vitality = Stat(5);
		player.strength = Stat(5);
		player.agility = Stat(5);
		player.intelligence = Stat(5);
	}
	else 	if (race == "Orc") {
		player.vitality = Stat(7);
		player.strength = Stat(8);
		player.agility = Stat(4);
		player.intelligence = Stat(3);
	}
	else 	if (race == "Elf") {
        player.vitality = Stat(4);
		player.strength = Stat(3);
		player.agility = Stat(7);
		player.intelligence = Stat(8);
	}
    player.crafting = Stat(1);
    player.fishing = Stat(1);
    player.InitializeStats();

	std::cout << "\nCharacter created:\n";
    std::cout << "Name: " << player.name << "\n";
    std::cout << "Race: " << player.race << "\n";
    std::cout << "Vitality: " << player.vitality.value << "\n";
    std::cout << "Strength: " << player.strength.value << "\n";
    std::cout << "Agility: " << player.agility.value << "\n";
    std::cout << "Intelligence: " << player.intelligence.value << "\n";
    std::cout << "\n";

	return player;
}


int Character::GetMaxHealth() const {
    return vitality.value * 15;
}

void Character::HealToFull() {
    currentHealth = GetMaxHealth();
}

int Character::GetMaxMana() const {
    return intelligence.value * 10;
}

void Character::ReplenishManaToFull() {
    currentMana = GetMaxMana();
}

void Character::InitializeStats() {
    currentHealth = GetMaxHealth();
    currentMana = GetMaxMana();
}

void Character::AddGold(int amount) {

	gold += amount;
}

void Character::AddItem(const Item& newItem) {
    if (newItem.data.consumable) { //check if consumable
        for (auto& item : inventory) { //to stack with existing item
            if (item.data.name == newItem.data.name) {
                item.data.quantity += newItem.data.quantity;
                return;
            }
        }
    }

    inventory.push_back(newItem); //add as new item if not stackable
}

bool Character::EquipItem(size_t index) {
    using IT = ItemType;

    if (index >= inventory.size()) {
        std::cout << "Invalid item index.\n";
        return false;
    }

    Item& item = inventory[index];

    if (!item.data.equippable) {
        std::cout << "That item can't be equipped.\n";
        return false;
    }

    IT slot = item.data.type;

    // Notify if replacing an existing item
    if (equipmentIndex.find(slot) != equipmentIndex.end()) {
        size_t oldIndex = equipmentIndex[slot];
        std::cout << "Unequipped " << inventory[oldIndex].data.name
            << " from " << GetSlotName(slot) << ".\n";
    }

    equipmentIndex[slot] = index;
    std::cout << "Equipped " << item.data.name << " to " << GetSlotName(slot) << ".\n";
    return true;
}

std::string Character::GetSlotName(ItemType type) const {
    switch (type) {
    case ItemType::MainHand: return "Main Hand";
    case ItemType::OffHand: return "Off Hand";
    case ItemType::Head: return "Head";
    case ItemType::Torso: return "Torso";
    case ItemType::Legs: return "Legs";
    case ItemType::Accessory: return "Accessory";
    default: return "Unknown";
    }
}

bool Character::UnequipItem(ItemType slot) {
    auto it = equipmentIndex.find(slot);
    if (it == equipmentIndex.end()) {
        std::cout << "\nNothing is equipped in that slot.\n";
        return false;
    }

    size_t index = it->second;
    if (index < inventory.size()) {
        std::cout << "\nUnequipped " << inventory[index].data.name
            << " from " << GetSlotName(slot) << ".\n";
    }

    equipmentIndex.erase(it);
    return true;
}

void Character::LearnSpell(const Spell& spell) {
    for (const auto& s : spells) {
        if (s.name == spell.name) return; // already known
    }
    spells.push_back(spell);
}

void Character::LearnAbility(const Ability& ability) {
    for (const auto& a : abilities) {
        if (a.name == ability.name) return; // already known
    }
    abilities.push_back(ability);
}

bool Character::HasDiscoveredLore(const std::string& content) const {
    for (const auto& entry : loreEntries) {
        if (entry.content == content) return true;
    }
    return false;
}

void Character::DiscoverLore(const LoreEntry& lore) {
    if (!HasDiscoveredLore(lore.content)) {
        loreEntries.push_back(lore);
        std::cout << "\nDiscovery unlocked: \"" << lore.content << "\"\n";
    }
    else {
        //std::cout << "\nYou've already uncovered this lore.\n";
    }
}

void Character::ViewAbilities() const {
	if (abilities.empty()) {
		std::cout << "\nYou haven't learned any abilities yet.\n";
		return;
	}

	std::cout << "=== ABILITIES ===\n";
	for (const auto& ability : abilities) {
		std::cout << "- " << ability.name << "\n"
			<< "  Description: " << ability.description << "\n"
			<< "  Usable in Combat: " << (ability.usableInCombat ? "Yes" : "No") << "\n"
			<< "  Usable Outside Combat: " << (ability.usableOutsideCombat ? "Yes" : "No") << "\n\n";
	}
}

void Character::ViewItems(bool allowUse, const std::string& context) {

    std::vector<size_t> visibleIndices = RefreshVisibleIndices(context);


    if (visibleIndices.empty()) {
        std::cout << "\nYou have no usable items.\n";
        return;
    }


    while (true) {

        DisplayInventory(visibleIndices);
        
        int choice;
        std::cin >> choice;

        if (choice == 0) break;
        if (choice < 1 || choice > static_cast<int>(visibleIndices.size())) {
            std::cout << "Invalid choice.\n";
            continue;
        }

        size_t selectedIndex = visibleIndices[choice - 1];
        Item& selected = inventory[selectedIndex];
        ItemType slot = selected.data.type;

        bool isSlotOccupied = equipmentIndex.find(slot) != equipmentIndex.end();
        bool isEquipped = isSlotOccupied && equipmentIndex.at(slot) == selectedIndex;

        InspectItem(selected);


        if (!allowUse) {
            std::cout << "\nPress any key to return.\n";
            std::cin.ignore();
            std::cin.get();
            continue;
        }

        int action = 0;

        if (selected.data.consumable && selected.data.equippable && isEquipped) {
            std::cout << "\n1. Use\n2. Unequip\n3. Back\n";
            std::cin >> action;
        }
        else if (selected.data.consumable && selected.data.equippable) {
            std::cout << "\n1. Use\n2. Equip\n3. Back\n";
            std::cin >> action;
        }
        else if (selected.data.equippable && isEquipped) {
            std::cout << "\n1. Unequip\n2. Back\n";
            std::cin >> action;
        }else if (selected.data.equippable && isSlotOccupied) {
            std::cout << "\n1. Replace\n2. Back\n";
            std::cin >> action;
        }
        else if (selected.data.equippable) {
            std::cout << "\n1. Equip\n2. Back\n";
            std::cin >> action;
        }
        else if (selected.data.consumable) {
            std::cout << "\n1. Use\n2. Back\n";
            std::cin >> action;
        }
        else {
            std::cout << "\nPress any key to return.\n";
            std::cin.ignore();
            std::cin.get();
            continue;
        }

        bool refresh = HandleItemAction(selected, isEquipped, action, context, selectedIndex);
        if (refresh) {
            visibleIndices = RefreshVisibleIndices(context);
            if (visibleIndices.empty()) {
                std::cout << "You have no usable items left.\n";
                break;
            }
            continue;
        }
    }
}

void Character::ViewEquipment() const {
    std::cout << "\n=== EQUIPMENT ===\n";

    // Define all possible equipment slots
    std::vector<ItemType> allSlots = {
        ItemType::MainHand,
        ItemType::OffHand,
        ItemType::Head,
        ItemType::Torso,
        ItemType::Legs,
        ItemType::Accessory
    };

    for (ItemType slot : allSlots) {
        std::cout << GetSlotName(slot) << ": ";

        auto it = equipmentIndex.find(slot);
        if (it != equipmentIndex.end() && it->second < inventory.size()) {
            const Item& item = inventory[it->second];
            std::cout << item.data.name << "\n";
        }
        else {
            std::cout << "(empty)\n";
        }
    }
}

SpellCastResult Character::ViewSpells(bool allowUse, const std::string& context) {
    std::vector<Spell*> visibleSpells;

    for (Spell& spell : spells) {
        if ((context == "combat" && spell.usableInCombat) ||
            (context == "interaction" && spell.usableOutsideCombat) ||
            (context == "menu")) {
            visibleSpells.push_back(&spell);
        }
    }

    if (visibleSpells.empty()) {
        std::cout << "\nYou have no usable spells.\n";
        return SpellCastResult::Failed;
    }

    while (true) {
        std::cout << "\n=== SPELL BOOK ===\n";
        for (size_t i = 0; i < visibleSpells.size(); ++i) {
            const Spell& spell = *visibleSpells[i];
            std::cout << i + 1 << ". " << spell.name << " \n";
        }
        std::cout << "0. Return\n=================\n";

        int choice;
        std::cin >> choice;

        if (choice == 0) break;
        if (choice < 1 || choice > static_cast<int>(visibleSpells.size())) {
            std::cout << "Invalid choice.\n";
            continue;
        }

        Spell& selected = *visibleSpells[choice - 1];
        InspectSpell(selected);

        if (!allowUse) {
            std::cout << "\nPress any key to return.\n";
            std::cin.ignore();
            std::cin.get();
            continue;
        }

        std::cout << "\n1. Cast\n2. Back\n";
        int action;
        std::cin >> action;

        if (action == 1) {
            SpellCastResult result = HandleSpellCast(selected, context);
            return result;
        }
    }

    return SpellCastResult::Failed;
}

void Character::InspectSpell(const Spell& spell) {

        std::cout << "\n=== " << spell.name << " ===\n";
        std::cout << spell.description << "\n";

        std::cout << "\n--- Spell Details ---\n";
        std::cout << "Mana Cost: " << spell.manaCost << "\n";
        std::cout << "Potency: " << spell.value << "\n";

    }

void Character::SetLastCastSpell(const Spell& spell) {
    lastCastSpell = spell;
    hasCastSpell = true;
}

const Spell* Character::GetLastCastSpell() const {
    return hasCastSpell ? &lastCastSpell : nullptr;
}

void Character::ViewLore() const {
    if (loreEntries.empty()) {
        std::cout << "\nYou haven't uncovered any lore yet.\n";
        return;
    }

    std::cout << "\n=== DISCOVERIES ===\n";
    for (size_t i = 0; i < loreEntries.size(); ++i) {
        std::cout << i + 1 << ". " << loreEntries[i].content << "\n";
    }

    std::cout << "\nPress any key to return.\n";
    std::cin.ignore();
    std::cin.get();
}

std::vector<size_t> Character::RefreshVisibleIndices(const std::string& context) {
    std::vector<size_t> indices;

    for (size_t i = 0; i < inventory.size(); ++i) {
        const auto& item = inventory[i];
        if (context == "combat" && !item.data.usableInCombat) continue;
        indices.push_back(i);
    }

    return indices;
}

void Character::DisplayInventory(const std::vector<size_t>& indices) {
    std::cout << "\n=== INVENTORY ===\n";
    for (size_t i = 0; i < indices.size(); ++i) {
        const auto& item = inventory[indices[i]];
        std::cout << i + 1 << ". " << item.data.name;

        // Show equipped status
        if (equipmentIndex.find(item.data.type) != equipmentIndex.end() &&
            equipmentIndex.at(item.data.type) == indices[i]) {
            std::cout << " [Equipped]";
        }

        // Show quantity for consumables
        if (item.data.consumable) {
            std::cout << " (" << item.data.quantity << ")";
        }

        std::cout << "\n";
    }
    std::cout << "0. Return";
    std::cout << "\n=================\n";
}

void Character::InspectItem(const Item& item) {
    std::cout << "\n=== " << item.data.name << " ===\n";
    std::cout << item.data.description << "\n";

    if (item.data.equippable) {
        std::cout << "\n--- Stat Bonuses ---\n";
        if (item.data.strengthBonus != 0)
            std::cout << "Strength: +" << item.data.strengthBonus << "\n";
        if (item.data.vitalityBonus != 0)
            std::cout << "Vitality: +" << item.data.vitalityBonus << "\n";
        if (item.data.agilityBonus != 0)
            std::cout << "Agility: +" << item.data.agilityBonus << "\n";
        if (item.data.intelligenceBonus != 0)
            std::cout << "Intelligence: +" << item.data.intelligenceBonus << "\n";
    }


    if (!item.data.loreEntry.empty()) {
        DiscoverLore(LoreEntry(item.data.loreEntry));
    }
}

bool Character::HandleItemAction(Item& item, bool isEquipped, int action, const std::string& context, size_t index) {

    bool used = false;

    // Handle Use
    if (item.data.consumable && (
        (item.data.equippable && action == 1) ||
        (!item.data.equippable && action == 1)
        )) {
        switch (item.data.type) {
        case ItemType::Healing:
            if (context == "combat" || context == "menu") {
                currentHealth = std::min(currentHealth + item.data.value, GetMaxHealth());
                std::cout << "You used " << item.data.name << " and restored " << item.data.value << " HP!\n";
                used = true;
            }
            else {
                std::cout << "You can't use that here.\n";
            }
            break;

        case ItemType::Mana:
            if (context == "combat" || context == "menu") {
                currentMana = std::min(currentMana + item.data.value, GetMaxMana());
                std::cout << "You used " << item.data.name << " and restored " << item.data.value << " MP!\n";
                used = true;
            }
            else {
                std::cout << "You can't use that here.\n";
            }
            break;

        case ItemType::Damage:
            if (context == "combat") {
                std::cout << "You used " << item.data.name << " and dealt " << item.data.value << " damage!\n";
                used = true;
                // Add enemy damage logic here
            }
            else {
                std::cout << "You can't use that outside of combat.\n";
            }
            break;

        case ItemType::Buff:
            if (context == "combat" || context == "menu") {
                std::cout << "You used " << item.data.name << ". Your stats feel empowered!\n";
                used = true;
                // Apply buff logic here
            }
            else {
                std::cout << "You can't use that here.\n";
            }
            break;

        case ItemType::Utility:
            if (context == "interaction") {
                std::cout << "You used " << item.data.name << ".\n";
                used = true;
                // Trigger interaction effect
            }
            else {
                std::cout << "That item doesn't do anything right now.\n";
            }
            break;

        default:
            std::cout << "That item can't be used.\n";
            break;
        }

        // Remove used consumable
        if (used) {
            if (item.data.quantity > 1) {
                item.data.quantity--;
            }
            else {
                if (equipmentIndex.find(item.data.type) != equipmentIndex.end() &&
                    equipmentIndex[item.data.type] == index) {
                    equipmentIndex.erase(item.data.type);
                }
                inventory.erase(inventory.begin() + index);
            }
            return true; // Inventory changed
        }
    }

    // Handle Equip
    if (item.data.equippable && !isEquipped && (
        (item.data.consumable && action == 2) ||
        (!item.data.consumable && action == 1)
        )) {
        if (EquipItem(index)) {
            std::cout << item.data.name << " equipped successfully.\n";
            return false;
        }
        else {
            std::cout << "Failed to equip " << item.data.name << ".\n";
        }
    }

    // Handle Unequip
    if (isEquipped && (
        (item.data.consumable && item.data.equippable && action == 2) ||
        (item.data.equippable && !item.data.consumable && action == 1)
        )) {
        if (UnequipItem(item.data.type)) {
            std::cout << item.data.name << " unequipped and returned to inventory.\n";
            return true;
        }
        else {
            std::cout << "Failed to unequip " << item.data.name << ".\n";
        }
    }

    return false; // No inventory change
}

SpellCastResult Character::HandleSpellCast(const Spell& spell, const std::string& context) {
    if (context == "combat" && !spell.usableInCombat) {
        std::cout << "This spell can't be used in combat.\n";
        return SpellCastResult::Failed;
    }
    if (context == "interaction" && !spell.usableOutsideCombat) {
        std::cout << "This spell can't be used outside of combat.\n";
        return SpellCastResult::Failed;
    }

    if (currentMana < spell.manaCost) {
        std::cout << "Not enough mana to cast " << spell.name << ".\n";
        return SpellCastResult::Failed;
    }

    currentMana -= spell.manaCost;
    std::cout << "You cast " << spell.name << "!\n";

    if (context == "combat" && spell.type == SpellType::Damage) {
        SetLastCastSpell(spell); // optional store for combat system
        return SpellCastResult::NeedsCombatDamage;
    }

    // Handle other spell types
    switch (spell.type) {
    case SpellType::Healing:
        currentHealth = std::min(currentHealth + spell.value, GetMaxHealth());
        std::cout << "You heal for " << spell.value << " HP!\n";
        return SpellCastResult::Success;

    case SpellType::Buff:
        std::cout << "You feel empowered!\n";
        return SpellCastResult::Success;

    case SpellType::Cleansing:
        //ClearStatusEffects();
        std::cout << "You feel purified.\n";
        return SpellCastResult::Success;

    case SpellType::Utility:
        std::cout << "You cast " << spell.name << ".\n";
        return SpellCastResult::Success;

    default:
        std::cout << "That spell can't be used.\n";
        return SpellCastResult::Failed;
    }
}

int Character::GetHealthRecoveryPerTurn() const {
    return vitality.value * 1; // 1 HP per vitality point per turn
}

int Character::GetManaRecoveryPerTurn() const {
    return intelligence.value * 1; // or tweak to 0.5f if needed
}

void Character::RecoverHealthAndManaPerTurn() {
    int healthRecovery = GetHealthRecoveryPerTurn();
    int manaRecovery = GetManaRecoveryPerTurn();

    currentHealth = std::min(currentHealth + healthRecovery, GetMaxHealth());
    currentMana = std::min(currentMana + manaRecovery, GetMaxMana());
}

int Character::GetTotalStrength() const {
    int total = strength.value;

    for (const auto& pair : equipmentIndex) {
        ItemType slot = pair.first;
        size_t index = pair.second;

        if (index < inventory.size()) {
            const Item& item = inventory[index];
            total += item.data.strengthBonus;
        }
    }
    return total;
}

int Character::GetTotalVitality() const {
    int total = vitality.value;

    for (const auto& pair : equipmentIndex) {
        ItemType slot = pair.first;
        size_t index = pair.second;

        if (index < inventory.size()) {
            const Item& item = inventory[index];
            total += item.data.vitalityBonus;
        }
    }
    return total;
 
}

int Character::GetTotalAgility() const {
    int total = agility.value;

    for (const auto& pair : equipmentIndex) {
        ItemType slot = pair.first;
        size_t index = pair.second;

        if (index < inventory.size()) {
            const Item& item = inventory[index];
            total += item.data.agilityBonus;
        }
    }
    return total;
}

int Character::GetTotalIntelligence() const {
    int total = intelligence.value;

    for (const auto& pair : equipmentIndex) {
        ItemType slot = pair.first;
        size_t index = pair.second;

        if (index < inventory.size()) {
            const Item& item = inventory[index];
            total += item.data.intelligenceBonus;
        }
    }
    return total;
}