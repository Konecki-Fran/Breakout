#pragma once

#include "IGame.h"

class BreakoutGame : public IGame
{
public:
	explicit BreakoutGame(Window& gameWindow, GameManager& gameManager, InputHandler& inputHandler, Level& level, SoundManager& soundManager);
	~BreakoutGame() final = default;

	GameObject& getObjectByTag(String tag) const;
	bool areAllBricksDestroyed() const;

	void initialize() override;
	void quit() override;

private:
	Window& _gameWindow;
	GameManager& _gameManager;
	InputHandler& _inputHandler;
	Level& _level;
	SoundManager& _soundManager;

	void physicsUpdate() override;
	void inputHandle() override;
	void update() override;
	void renderScene() override;

	void checkBallWindowCollision() const;
	bool isLifeLost() const;
	void resetBallPosition() const;
	void resetPlayerPosition() const;

};

