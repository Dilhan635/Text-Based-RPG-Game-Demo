#include "CombatSystem.h"
#include "Character.h"
#include "Dialogue.h"
#include <iostream>
#include <thread> //for sleep_for
#include "GameEngine.h"
#include "CombatState.h"

void CombatSystem::ShowPreBattleScreen(GameEngine& engine, Character& player, Enemy& enemy) {
	std::cout << "\nYou encounter a " << enemy.name << "!\n";
	std::cout << "Do you want to [E] engage or [A] avoid?\n";

	char choice;
	std::cin >> choice;

	if (choice == 'e' || choice == 'E') {
		std::cout << "You prepare for battle! \n";
		//result = StartCombat(player, enemy, true);
		engine.ChangeState(new CombatState(player, enemy, true));
	}
	else if (choice == 'a' || choice == 'A') {
		if (CombatSystem::AttemptEscape(player, enemy)) {
			std::cout << "You successfully avoided the encounter.\n";
			return;
		}
		else {
			std::cout << "You failed to escape! The " << enemy.name << " approaches!\n";
			//result = StartCombat(player, enemy, false);
			engine.ChangeState(new CombatState(player, enemy, false));
		}
	}
	else {
		std::cout << "You hesitate... The " << enemy.name << " approaches!\n";
		//result = StartCombat(player, enemy, false);
		engine.ChangeState(new CombatState(player, enemy, false));
	}

}
/*
CombatResult CombatSystem::StartCombat(Character& player, Enemy& enemy, bool playerFirst) {
	CombatResult result = CombatResult::DEFEAT; // default outcome

	enemy.personality = GetRandomPersonality();
	bool talkedToEnemy = false;
	bool combatOngoing = true;

	if (!playerFirst && player.currentHealth > 0 && enemy.currentHealth > 0) {
		HandleEnemyTurn(player, enemy, 0);
		std::cout << "\n-----------------------------\n";
	}

	while (combatOngoing) {
		int choice = GetPlayerChoice();
		HandlePlayerAction(choice, player, enemy, talkedToEnemy, combatOngoing);

		// Escape logic
		if (choice == 7) {
			if (AttemptEscape(player, enemy)) {
				result = CombatResult::ESCAPE;
				combatOngoing = false;
			}
		}

		if (enemy.currentHealth <= 0) {
			std::cout << "You felled the " << enemy.name << "!\n";
			result = CombatResult::VICTORY;
			combatOngoing = false;
		}
		else if (player.currentHealth <= 0) {
			result = CombatResult::DEFEAT;
			combatOngoing = false;
		}
		else {
			if (combatOngoing == player.currentHealth > 0 && enemy.currentHealth > 0) {
				HandleEnemyTurn(player, enemy, choice);
				std::cout << "\n-----------------------------\n";
			}

		}
	}

	return result;
}
*/
void CombatSystem::WinScreen(Character& player, const Enemy& defeatedEnemy) {
	int goldEarned = defeatedEnemy.goldReward;
	player.AddGold(goldEarned);

	std::vector<const ItemData*> loot = defeatedEnemy.RollLoot();

	std::cout << "\n=============================\n";
	std::cout << "VICTORY!\n";
	std::cout << "Gold: " << goldEarned << "\n";

	if (!loot.empty()) {
		std::cout << "Loot:\n";
		for (const ItemData* itemData : loot) {
			Item item(*itemData);
			player.AddItem(item);
			std::cout << "- " << item.data.name << "\n"; // display item name	
		}
	}
	else {
		std::cout << "No loot dropped.\n";
	}

	std::cout << "=============================\n";
}

bool CombatSystem::AttemptEscape(Character& player, const Enemy& enemy) {
	float escapeChance = static_cast<float>(player.GetTotalAgility()) /
		(player.GetTotalAgility() + enemy.agility);
	float roll = static_cast<float>(rand()) / RAND_MAX;

	std::cout << "You attempt to escape...\n";
	Pause(600);

	if (roll < escapeChance) {
		std::cout << "You successfully escaped!\n\n";
		player.agility.GainXP(CalculateXP("agility", enemy));
		return true;
	}
	else {
		std::cout << "You failed to escape!\n";
		return false;
	}
}

int CombatSystem::GetPlayerChoice() {
	std::cout << "\nYour Turn! Select an Action:\n";
	Pause(400);
	std::cout << "1. Attack\n";
	Pause(200);
	std::cout << "2. Defend\n";
	Pause(200);
	std::cout << "3. Spells\n";
	Pause(200);
	std::cout << "4. Abilities\n";
	Pause(200);
	std::cout << "5. Talk\n";
	Pause(200);
	std::cout << "6. Item\n";
	Pause(200);
	std::cout << "7. Escape\n";
	Pause(200);

	int choice;
	while (true) {
		if (std::cin >> choice && choice >= 1 && choice <= 7) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return choice;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input, please enter a number 1-7.\n";
	}

	return choice;
}

void CombatSystem::HandlePlayerAction(
	int choice,
	Character& player,
	Enemy& enemy,
	bool& talkedToEnemy,
	bool& combatOngoing)
{
	switch (choice) {
	case 1: { // Attack
		float hitChance = static_cast<float>(player.GetTotalAgility()) /
			(player.GetTotalAgility() + enemy.agility);
		float roll = static_cast<float>(rand()) / RAND_MAX;

		std::cout << "You attempt to strike " << enemy.name << "...\n";
		Pause(600);

		if (roll < hitChance) {
			int die1 = (rand() % 6) + 1;
			int die2 = (rand() % 6) + 1;
			int damage = die1 + die2 + player.GetTotalStrength();
			bool isCritical = (die1 == die2);

			if (isCritical) {
				damage *= 2;
				std::cout << "Critical Hit!\n";
			}

			enemy.currentHealth -= damage;
			std::cout << "It takes " << damage << " damage!\n";
			player.strength.GainXP(CalculateXP("strength", enemy));
		}
		else {
			std::cout << "Your attack misses!\n";
		}
		break;
	}
	case 2: { // Defend
		std::cout << "You brace for the next attack.\n";
		break;
	}
	case 3: { // Spells
		SpellCastResult result = player.ViewSpells(true, "combat");
		if (result == SpellCastResult::NeedsCombatDamage) {
			const Spell* spell = player.GetLastCastSpell();
			if (spell) {
				int damage = CalculateSpellEffect(*spell, player);
				enemy.currentHealth -= damage;
				std::cout << "It deals " << damage << " damage to " << enemy.name << "!\n";
				int spellValue = spell->value;
				player.intelligence.GainXP(CalculateXP("intelligence", enemy, spellValue));
				std::cout << "Your Mana: " << player.currentMana << "\n";
			}
		}
		break;
	}
	case 4: { // Abilities
		// Future: HandleAbilityUse(player, enemy);
		break;
	}
	case 5: { // Talk
		if (talkedToEnemy) {
			std::cout << "You've already spoken to " << enemy.name << ".\n";
			break;
		}

		TalkResult result = TalkToEnemy(enemy, player.GetTotalIntelligence());
		std::cout << result.message << "\n";
		talkedToEnemy = true;

		switch (result.outcome) {
		case TalkOutcome::FLEE:
			enemy.currentHealth = 0; // Ends combat cleanly
			combatOngoing = false;
			break;
		case TalkOutcome::TEACH_ABILITY:
			std::cout << "You learned a new ability: " << enemy.ability << "!\n";
			break;
		case TalkOutcome::GIVE_ITEM:
			std::cout << "You received a rare item!\n";
			break;
		case TalkOutcome::REVEAL_LORE:
			// Optional: unlock lore system
			break;
		case TalkOutcome::ATTACK:
		case TalkOutcome::NONE:
		default:
			break;
		}
		break;
	}
	case 6: { // Item
		player.ViewItems(true, "combat");
		break;
	}
	case 7: { // Escape
		// Escape is handled in CombatState::Update, so do nothing here
		break;
	}
	default:
		std::cout << "Invalid choice, you lose a turn!\n";
		break;
	}
	Pause(600);
}

void CombatSystem::HandleEnemyTurn(Character& player, Enemy& enemy, int lastPlayerChoice) {
	std::cout << "\nThe " << enemy.name << "'s Turn!\n";
	Pause(600);

	// Step 1: Dodge check
	int dodgeRoll = rand() % 100;
	int dodgeChance = player.GetTotalAgility() * 5;
	bool defended = (lastPlayerChoice == 2);

	if (dodgeRoll < dodgeChance) {
		std::cout << "You dodged the attack!\n";
		player.agility.GainXP(CalculateXP("agility", enemy));
		Pause(600);
		return;
	}

	// Step 3: Damage roll
	int die1 = (rand() % 6) + 1;
	int die2 = (rand() % 6) + 1;
	int baseDamage = die1 + die2;
	bool isCritical = (die1 == die2);

	int damage = (enemy.type == "Magic")
		? baseDamage + enemy.intelligence
		: baseDamage + enemy.strength;

	if (isCritical) {
		damage *= 2;
		std::cout << "Critical Hit!\n";
		player.vitality.GainXP(CalculateXP("vitality", enemy) / 2);
	}

	int defense = player.GetTotalVitality() / 2;
	damage = std::max(1, damage - defense);

	// Step 4: Defend modifier
	if (defended) {
		damage /= 2;
		player.vitality.GainXP(CalculateXP("vitality", enemy));
	}

	// Step 5: Apply damage
	CombatSystem::TakeDamage(player, damage);
	player.vitality.GainXP(CalculateXP("vitality", enemy));

	Pause(600);
}

void CombatSystem::TakeDamage(Character& player, int amount) {
	player.currentHealth -= amount;
	std::cout << "You take " << amount << " damage!\n";
	std::cout << "Your Health: " << player.currentHealth << "\n";

	Pause(600);

	if (player.currentHealth <= 0) {
		std::cout << "You have been defeated...\n";
		Pause(600);
		// Optional: trigger game over logic here
	}
}

void CombatSystem::Pause(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int CombatSystem::RollDice(int numberOfDice, int sidesPerDie) {
	int total = 0;
	for (int i = 0; i < numberOfDice; ++i) {
		total += (rand() % sidesPerDie) + 1; // roll from 1 to sidesPerDie
	}
	return total;
}

int CombatSystem::CalculateXP(const std::string& playerStat, const Enemy& enemy, int spellValue) {

	if (playerStat == "strength") {
		return enemy.vitality;
	}
	else if (playerStat == "agility") {
		return enemy.agility;
	}
	else if (playerStat == "vitality") {
		return (enemy.type == "Magic") ? enemy.intelligence : enemy.strength;
	}
	else if (playerStat == "intelligence") {
		return enemy.intelligence + spellValue / 2;
	}
	return 0;
}

int CombatSystem::CalculateSpellEffect(const Spell& spell, const Character& caster) {
	// Roll two dice (2d6)
	int die1 = (rand() % 6) + 1;
	int die2 = (rand() % 6) + 1;
	int baseRoll = die1 + die2;

	// Check for critical hit
	bool isCritical = (die1 == die2);

	// Intelligence scaling
	int intelligenceBonus = caster.GetTotalIntelligence(); // Assuming you have this method
	int damage = baseRoll + intelligenceBonus;

	if (isCritical) {
		damage *= 2;
		std::cout << "Critical Hit!\n";
	}

	return damage;
}
