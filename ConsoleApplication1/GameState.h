#pragma once

class GameEngine; // Forward declare. Don't need the full header

class GameState {

public:

	virtual void Enter(GameEngine& engine) = 0;
	virtual void Exit(GameEngine& engine) = 0;
	virtual void Update(GameEngine& engine) = 0;
	virtual void Render(GameEngine& engine) = 0;
	virtual ~GameState() {}
};