#include "ExplorationState.h"
#include "CombatState.h"
#include "GameEngine.h"
#include <iostream>

ExplorationState::ExplorationState(Character& playerRef)
    : player(playerRef) {
}

void ExplorationState::Enter(GameEngine& engine) {

}

void ExplorationState::Exit(GameEngine& engine) {

}

void ExplorationState::Update(GameEngine& engine) {
    std::cout << "WASD. Move\nI. Inspect\nM. Map\nC. Stats\nE. Menu\nQ. Quit.\n";
    char input;
    std::cin >> input;
    input = std::tolower(input);

    int newX = engine.IsInLocalMap() ? engine.GetLocalX() : engine.GetOverworldX();
    int newY = engine.IsInLocalMap() ? engine.GetLocalY() : engine.GetOverworldY();

    std::string direction;

    switch (input) {
    case 'w': newY--; direction = "north"; break;
    case 's': newY++; direction = "south"; break;
    case 'a': newX--; direction = "west"; break;
    case 'd': newX++; direction = "east"; break;

    case 'i':
        engine.InspectTile();
        return;
    case 'm':
        std::cout << "\nYou take a look at your map. A moment slips by...\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        engine.Render();
        engine.AdvanceGameTime();
        engine.GetPlayer().RecoverHealthAndManaPerTurn();
        engine.HandleRandomEncounter();
        engine.HandleBossEncounter();
        return;

    case 'c':
        engine.ShowStatus();
        return;

    case 'e':
        engine.ShowMenu();
        return;

    case 'q':
        engine.IsRunning() = false;
        return;

    default:
        std::cout << "\nUnknown input.\n";
        return;
    }

    if (engine.TryMovePlayer(newX, newY, direction)) {
        engine.AdvanceGameTime();
        engine.GetPlayer().RecoverHealthAndManaPerTurn();
        engine.HandleRandomEncounter();
        engine.HandleBossEncounter();
    }
}

void ExplorationState::Render(GameEngine& engine){

}