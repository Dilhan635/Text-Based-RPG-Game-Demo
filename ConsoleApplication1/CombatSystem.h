#pragma once
#include "Character.h"
#include "Enemy.h"
#include "CombatResult.h"
#include <thread>

class GameEngine;

class CombatSystem {
public:
    static void ShowPreBattleScreen(GameEngine& engine, Character& player, Enemy& enemy);
    static CombatResult StartCombat(Character& player, Enemy& enemy, bool playerFirst); // returns an Enum
    static void TakeDamage(Character& player, int amount);
    static void WinScreen(Character& player, const Enemy& defeatedEnemy);

    static bool AttemptEscape(Character& player, const Enemy& enemy);

    static int GetPlayerChoice();
    static void HandlePlayerAction(int choice, Character& player, Enemy& enemy, bool& talkedToEnemy, bool& combatVictory);
    static void HandleEnemyTurn(Character& player, Enemy& enemy, int lastPlayerChoice);

    static void Pause(int ms);

    static int RollDice(int numberOfDice, int sidesPerDie);

    static int CalculateXP(const std::string& statType, const Enemy& enemy, int spellValue = 0);

    static int CalculateSpellEffect(const Spell& spell, const Character& caster);
};

