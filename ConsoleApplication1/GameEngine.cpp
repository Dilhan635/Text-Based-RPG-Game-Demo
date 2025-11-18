#include "GameEngine.h"
#include "ExplorationState.h"
#include "Character.h"
#include "Ability.h"
#include "Item.h"
#include "Loreentry.h"
#include "Enemy.h"
#include "Dialogue.h"
#include "CombatSystem.h"
#include "ItemDatabase.h"
#include "SpellDatabase.h"
#include "Overworld.h"
#include "LocalGrid.h"
#include "LocalGridGenerator.h"
#include "EnemyPool.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cctype>

using namespace std;

void GameEngine::Init(const Character& c) {
	// Generate 100x100 grid for overworld
	overworld = std::move(GenerateOverworld(100, 100));

	// Set player start position in overworld
	overworldX = 10;
	overworldY = 10;

	// Ensure the starting tile has a local map
	auto& startTile = overworld.map[overworldY][overworldX];
	if (!startTile.localMap) {
		startTile.localMap = GenerateSimpleDungeon(40, 40);
	}

	// Enter the local map using context system
	EnterLocalMap(startTile.localMap, 4, 2);

	// Assign initialized player
	player = c;
	isRunning = true;

	// Load starting inventory
	LoadStartingInventory();
	ChangeState(new ExplorationState(player));

}

void GameEngine::Run() {

	while (isRunning && currentState) {
		bool tookTurn = ProcessInput();
		currentState->Update(*this);
		currentState->Render(*this);
	}
}

void GameEngine::ShutDown() {
	cout << "Game Shutdown\n";
}

bool GameEngine::ProcessInput() {

	std::cout << "WASD. Move\nI. Inspect\nM. Map\nC. Stats\nE. Menu\nQ. Quit.\n";
	char input;
	std::cin >> input;
	input = std::tolower(input); //normalise to lowercase

	int newX = inLocalMap ? localX : overworldX;
	int newY = inLocalMap ? localY : overworldY;


	string direction = "";

	bool tookTurn = false;

	switch (input) {
	case 'w': newY--; direction = "north"; break;
	case 's': newY++; direction = "south"; break;
	case 'a': newX--; direction = "west"; break;
	case 'd': newX++; direction = "east"; break;

	case 'i' :
		Inspect();
		return false;

	case 'm':
		std::cout << "\nYou take a look at your map. A moment slips by...\n";
		std::cout << "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
		Render(); // displays map
		AdvanceTime(); // contributes to an advancement in time
		player.RecoverHealthAndManaPerTurn();
		return true;
	case 'c':
		StatusScreen(); // displays status screen
		return false;
	case 'e':
		OpenMenu();
		return false;
	case 'q':
		isRunning = false;
		return false;

	default: std::cout << "\nUnknown input.\n";
		break;
	}

	return TryMove(newX, newY, direction);
	}

bool GameEngine::TryMove(int newX, int newY, const std::string& direction) {
	if (inLocalMap) {
		LocalGrid* grid = GetCurrentMap();
		if (!grid || newY < 0 || newY >= grid->height || newX < 0 || newX >= grid->width)
			return false;

		auto& destinationTile = grid->tiles[newY][newX];
		if (!destinationTile.walkable) {
			std::cout << "\nYour path is blocked...\n\n";
			return false;
		}

		localX = newX;
		localY = newY;
		std::cout << "\nYou moved " << direction << ".\n\n";
		AdvanceTime();
		player.RecoverHealthAndManaPerTurn();

		if (destinationTile.type == LocalTileType::Exit) {
			std::cout << "\nYou step into the overworld...\n\n";
			ExitLocalMap();
		}
		return true;
	}
	else {
		if (newY < 0 || newY >= overworld.height || newX < 0 || newX >= overworld.width)
			return false;

		auto& tile = overworld.map[newY][newX];
		if (!tile.isWalkable) {
			std::cout << "\nThe " << tile.name << " blocks your path.\n\n";
			return false;
		}

		overworldX = newX;
		overworldY = newY;
		std::cout << "\nYou moved " << direction << ".\n\n";
		AdvanceTime();
		player.RecoverHealthAndManaPerTurn();

		// Optional: auto-enter dungeon if standing on one
		if (tile.localMap) {
			std::cout << "\nYou descend into the dungeon...\n\n";
			EnterLocalMap(tile.localMap, 4, 2);
		}

		return true;
	}
}

void GameEngine::Render() {

	if (inLocalMap) {
		RenderLocalMap();
	}
	else {
		RenderOverworldMap();
	}
}

void GameEngine::AdvanceTime() {
	stepsSinceLastTimeChange++;
	if (stepsSinceLastTimeChange >= stepsPerTimeSlot) {
		currentTime.AdvanceTime();
		std::cout << "Time passes... It is now " << currentTime.GetTimeString() << ".\n\n";
		stepsSinceLastTimeChange = 0;
	}
}

void GameEngine::StatusScreen() const {

	cout << "\n========== STATUS SCREEN ==========\n";
	cout << player.name << " the " << player.race << "\n";
	cout << "Health: " << player.currentHealth << "\n";
	cout << "Mana: " << player.currentMana << "\n";
	cout << "----------\n";
	cout << "Vitality: " << player.vitality.value << " (XP: " << player.vitality.xp << "/" << player.vitality.xpToNextLvl << ")\n";
	cout << "Strength: " << player.strength.value << " (XP: " << player.strength.xp << "/" << player.strength.xpToNextLvl << ")\n";
	cout << "Agility: " << player.agility.value << " (XP: " << player.agility.xp << "/" << player.agility.xpToNextLvl << ")\n";
	cout << "Intelligence: " << player.intelligence.value << " (XP: " << player.intelligence.xp << "/" << player.intelligence.xpToNextLvl << ")\n";
	cout << "----------\n";
	cout << "Crafting: " << player.crafting.value << " (XP: " << player.crafting.xpToNextLvl << ")\n";
	cout << "Fishing: " << player.fishing.value << " (XP: " << player.fishing.xpToNextLvl << ")\n";
	cout << "----------\n";
	cout << "Gold: " << player.gold << "\n";
	cout << "===================================\n\n";
}

void GameEngine::OpenMenu() {

	char choice;
	do {
		cout << "\n===== MENU =====\n";
		cout << "1. Equipment\n";
		cout << "2. Spells\n";
		cout << "3. Abilities\n";
		cout << "4. Inventory\n";
		cout << "5. Craft\n";
		cout << "6. Discoveries\n";
		cout << "7. Save Game\n";
		cout << "8. Load Game\n";
		cout << "0. Return to Game\n";
		cout << "================\n";
		cin >> choice;

		switch (choice) {
		case '1':
			player.ViewEquipment();
			break;
		case '2':
			player.ViewSpells();
			break;
		case '3':
			player.ViewAbilities();
			break;
		case '4':
			player.ViewItems(true, "menu");
			break;
		case '5':
			//player.Craft();
			break;
		case '6':
			player.ViewLore();
			break;
		case '7':
			// Save
			break;
		case '8':
			// Load
			break;
		case '0':
			// Returning to game
			break;
		default:
			cout << "Invalid choice.\n";
		}
	} while (choice != '0');
}

void GameEngine::LoadStartingInventory() {
	player.AddItem(Item(ItemDatabase::FadedLetter));
}

void GameEngine::Inspect() {
	auto& currentTile = overworld.map[overworldY][overworldX];
	auto* grid = currentTile.localMap;

	if (!grid) {
		std::cout << "\nThere's nothing to inspect here.\n\n";
		return;
	}

	auto& tile = grid->tiles[localY][localX];

	if (tile.type == LocalTileType::Item && !tile.itemName.empty()) {
		// Try item lookup first
		auto itemIt = ItemDatabase::itemMap.find(tile.itemName);
		if (itemIt != ItemDatabase::itemMap.end()) {
			Item item(*itemIt->second);
			player.AddItem(item);
			std::cout << "\nYou picked up the " << item.data.name << "!\n\n";
		}
		else {
			// Try spell lookup
			auto spellIt = SpellDatabase::spellMap.find(tile.itemName);
			if (spellIt != SpellDatabase::spellMap.end()) {
				Spell spell(*spellIt->second);
				player.LearnSpell(spell);
				std::cout << "\nYou learned the " << spell.name << " Spell!\n\n";
			}
			else {
				std::cout << "\nInvalid Item or Spell: " << tile.itemName << "\n\n";
			}
		}

		// Clear tile
		tile.itemName = "";
		tile.type = LocalTileType::Floor;
		return;
	}


	//handle gold pickup
	if (tile.type == LocalTileType::Gold && tile.goldAmount > 0) {
		player.AddGold(tile.goldAmount);
		std::cout << "\nYou found " << tile.goldAmount << " Gold!\n\n";

		//setting it back to 0
		tile.goldAmount = 0;
		tile.type = LocalTileType::Floor;
		return;
	}

	// handle NPC interaction
	if (tile.type == LocalTileType::NPC && tile.hasNPC) {
		std::cout << "\n";
		tile.npc.Interact(player); // This handles name, description, and dialogue
		std::cout << "\n";
		return;
	}

	//default inspect
	std::cout << "\n";
	std::cout << tile.description << "\n";
}

void GameEngine::TryRandomEncounter() {

	if (!enemyPoolPtr || enemyPoolPtr->empty()) return; // Safety check

	if (rand() % 100 < 100) {
		int index = rand() % enemyPoolPtr->size();
		Enemy& randomEnemy = (*enemyPoolPtr)[index];

		if (randomEnemy.isBoss) return;

		randomEnemy.x = localX;
		randomEnemy.y = localY;

		if (randomEnemy.currentHealth <= 0) {
			randomEnemy.currentHealth = randomEnemy.maxHealth;
		}

		CombatSystem::ShowPreBattleScreen(*this, player, randomEnemy);
	}
}

void GameEngine::TryBossEncounter() {

	auto& startTile = overworld.map[overworldY][overworldX];
	LocalGrid* grid = startTile.localMap;

	if (!grid || grid->tiles.empty()) return;
	if (localY < 0 || localY >= grid->tiles.size()) return;
	if (localX < 0 || localX >= grid->tiles[localY].size()) return;

	LocalTile& tile = grid->tiles[localY][localX];

		if (tile.enemy && tile.enemy->isBoss && tile.enemy->currentHealth > 0) {
			cout << tile.enemy->name << " stands before you.\n";
			//CombatSystem::StartCombat(player, *tile.enemy, true);
		}
	}

void GameEngine::EnterLocalMap(LocalGrid* dungeon, int entryX, int entryY) {
	mapStack.push({ false, overworldX, overworldY, localX, localY, nullptr });
	inLocalMap = true;
	localX = entryX;
	localY = entryY;
	currentLocalMap = dungeon;
	enemyPoolPtr = &dungeon->enemyPool;
}

void GameEngine::ExitLocalMap() {
	if (!mapStack.empty()) {
		auto context = mapStack.top();
		mapStack.pop();
		inLocalMap = context.isLocal;
		overworldX = context.overworldX;
		overworldY = context.overworldY;
		localX = context.localX;
		localY = context.localY;
		currentLocalMap = context.localMap;
		enemyPoolPtr = nullptr;
	}
}

LocalGrid* GameEngine::GetCurrentMap() {
	return inLocalMap ? currentLocalMap : nullptr;
}

void GameEngine::RenderLocalMap() {
	LocalGrid* grid = GetCurrentMap();
	if (!grid) {
		std::cout << "No local map loaded.\n";
		return;
	}
	if (localY < 0 || localY >= grid->height || localX < 0 || localX >= grid->width) {
		std::cout << "Invalid local coordinates!\n";
		return;
	}

	int viewRadius = 7;
	int minY = std::max(0, localY - viewRadius); // Clamps to avoid going above the top edge of the map
	int maxY = std::min(grid->height - 1, localY + viewRadius);
	int minX = std::max(0, localX - viewRadius);
	int maxX = std::min(grid->width - 1, localX + viewRadius);

	std::cout << "========= LOCAL VIEW =========\n";
	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			if (x == localX && y == localY) {
				std::cout << " P";
			}
			else {
				LocalTile& tile = grid->tiles[y][x];
				switch (tile.type) {
				case LocalTileType::Wall: std::cout << " #"; break;
				case LocalTileType::Floor: std::cout << " ."; break;
				case LocalTileType::Door: std::cout << " D"; break;
				case LocalTileType::NPC: std::cout << " N"; break;
				case LocalTileType::Item: std::cout << " o"; break;
				case LocalTileType::Gold: std::cout << " *"; break;
				case LocalTileType::Exit: std::cout << " _"; break;
				default: std::cout << " .";
				}
			}
		}
		std::cout << "\n";
	}
	std::cout << "=============================\n\n";
}

void GameEngine::RenderOverworldMap() {
	int viewRadius = 40;
	int minY = std::max(0, overworldY - viewRadius);
	int maxY = std::min(overworld.height - 1, overworldY + viewRadius);
	int minX = std::max(0, overworldX - viewRadius);
	int maxX = std::min(overworld.width - 1, overworldX + viewRadius);

	std::cout << "======= OVERWORLD VIEW =======\n";
	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			if (x == overworldX && y == overworldY) {
				std::cout << " P";
			}
			else {
				OverworldTile& tile = overworld.map[y][x];

				switch (tile.type) {
				case TileType::Plains: std::cout << " ."; break;
				case TileType::Forest: std::cout << "::"; break;
				case TileType::Mountain: std::cout << "MM"; break;
				case TileType::Water: std::cout << "~~"; break;
				case TileType::Town:
					if (tile.name == "Town Building") std::cout << "[]";
					else std::cout << " ."; // Plain walkable tile
					break;
				case TileType::Dungeon: std::cout << " _"; break;
				default: std::cout << " ?"; break;
				}
				// You can customize this based on tile type
				//if (tile.localMap) {
				//	std::cout << " _"; // Dungeon entrance
				//}
				//else {
				//	std::cout << " ."; // Default terrain
				//}
			}
		}
		std::cout << "\n";
	}
	std::cout << "=============================\n\n";
}

void GameEngine::ChangeState(GameState* newState) {
	if (currentState) {
		currentState->Exit(*this);
		delete currentState;
	}
	currentState = newState;
	currentState->Enter(*this);
}

void GameEngine::Update() {
	if (currentState) {
		currentState->Update(*this);
	}
}

/*
void GameEngine::Render() {
	if (currentState) {
		currentState->Render(*this);
	}
}
*/

GameEngine::~GameEngine() {
	delete currentState;
}