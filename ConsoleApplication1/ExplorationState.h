#pragma once
#include "GameState.h"
#include "Character.h"

class GameEngine;

class ExplorationState : public GameState {
private:
    Character& player;

public:
    ExplorationState(Character& playerRef);

    void Enter(GameEngine& engine) override;
    void Exit(GameEngine& engine) override;
    void Update(GameEngine& engine) override;
    void Render(GameEngine& engine) override;
};
