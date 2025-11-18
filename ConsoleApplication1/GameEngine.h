#pragma once
#include "GameState.h"
#include "Character.h"
#include "Enemy.h"
#include "Overworld.h"
#include "LocalGrid.h"
#include "GameTime.h"
#include <vector>
#include <stack>

struct MapContext { // All members are public because Struct
	bool isLocal;
	int overworldX, overworldY;
	int localX, localY;
	LocalGrid* localMap;
};

class GameEngine {

public:
	void Init(const Character& c);
	void Run();
	void ShutDown();

	void ChangeState(GameState* newState);
	void Update();
	void Render();
	~GameEngine();

	// Accessors
	bool IsInLocalMap() const { return inLocalMap; }
	int GetLocalX() const { return localX; }
	int GetLocalY() const { return localY; }
	int GetOverworldX() const { return overworldX; }
	int GetOverworldY() const { return overworldY; }
	Character& GetPlayer() { return player; }
	bool& IsRunning() { return isRunning; }

	// Proxy methods
	void AdvanceGameTime() { AdvanceTime(); }
	bool TryMovePlayer(int x, int y, const std::string& dir) { return TryMove(x, y, dir); }
	void ShowStatus() const { StatusScreen(); }
	void ShowMenu() { OpenMenu(); }
	void InspectTile() { Inspect(); }
	void HandleRandomEncounter() { TryRandomEncounter(); }
	void HandleBossEncounter() { TryBossEncounter(); }


private:

	GameState* currentState = nullptr;

	Character player;
	Overworld overworld;

	//game loop
	bool isRunning = false;

	//position tracking
	int overworldX = 10; // Starting tile
	int overworldY = 10;
	int localX = 1;      // Position inside local grid
	int localY = 1;
	bool inLocalMap = false;

	std::stack<MapContext> mapStack;

	LocalGrid* currentLocalMap;

	LocalGrid* GetCurrentMap();

	void EnterLocalMap(LocalGrid* dungeon, int entryX, int entryY);
	void ExitLocalMap();

	bool TryMove(int newX, int newY, const std::string& direction);

	// Time management
	GameTime currentTime;
	void AdvanceTime();
	int stepsSinceLastTimeChange = 0;
	const int stepsPerTimeSlot = 20;

	// Encounters
	std::vector<Enemy>* enemyPoolPtr = nullptr;
	void TryRandomEncounter();
	void TryBossEncounter();

	void LoadStartingInventory();
	bool ProcessInput();

	void RenderLocalMap();
	void RenderOverworldMap();
	void StatusScreen() const; // read-only
	void OpenMenu();
	void Inspect();

};

