#pragma once
#include "GameState.h"
#include "CombatSystem.h"
#include "Character.h"
#include "Enemy.h"

class GameEngine;

class CombatState : public GameState {
private:
	Character& player;
	Enemy& enemy;
	bool playerGoesFirst;
	//bool combatVictory = false;
	bool talkedToEnemy = false;
	//bool combatStarted = true
	bool combatOngoing = true; // Starts active

public:
	CombatState(Character& p, Enemy& e, bool first); 

	void Enter(GameEngine& engine) override;
	void Exit(GameEngine& engine) override;
	void Update(GameEngine& engine) override;
	void Render(GameEngine& engine) override;
};