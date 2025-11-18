#include "CombatState.h"
#include "ExplorationState.h"
#include "GameEngine.h"

CombatState::CombatState(Character& playerRef, Enemy& enemyRef, bool first)
    : player(playerRef), enemy(enemyRef), playerGoesFirst(first), combatOngoing(true) {

}

void CombatState::Enter(GameEngine& engine) {
    std::cout << "Entering combat...\n";
    enemy.personality = GetRandomPersonality();

    if (!playerGoesFirst && player.currentHealth > 0 && enemy.currentHealth > 0) {
        CombatSystem::HandleEnemyTurn(player, enemy, 0);
        std::cout << "\n-----------------------------\n";
    }

    // Immediately prompt the player
    Update(engine);
}

void CombatState::Exit(GameEngine& engine) {

}

void CombatState::Update(GameEngine& engine) {
    while (combatOngoing) {
        int choice = CombatSystem::GetPlayerChoice();
        CombatSystem::HandlePlayerAction(choice, player, enemy, talkedToEnemy, combatOngoing);

        // Escape check
        if (choice == 7 && CombatSystem::AttemptEscape(player, enemy)) {
            engine.ChangeState(new ExplorationState(player));
            combatOngoing = false;
            break;
        }

        // Victory check
        if (enemy.currentHealth <= 0) {
            std::cout << "You felled the " << enemy.name << "!\n";
            CombatSystem::WinScreen(player, enemy);
            engine.ChangeState(new ExplorationState(player));
            combatOngoing = false;
            break;
        }

        // Defeat check
        if (player.currentHealth <= 0) {
            std::cout << "You have been defeated...\n";
            combatOngoing = false;
            break;
        }

        // Enemy turn
        CombatSystem::HandleEnemyTurn(player, enemy, choice);
        std::cout << "\n-----------------------------\n";
    }
}

void CombatState::Render(GameEngine& engine) {

}