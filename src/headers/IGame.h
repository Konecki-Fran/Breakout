#pragma once

#include "Window.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "Level.h"
#include "GameEvent.h"

#include <memory>
#include <optional>

class IGame
{
public:
	virtual ~IGame() = default;

	virtual void initialize() = 0;
	void updateFrame() 
	{
		physicsUpdate();
		inputHandle();
		update();
		renderScene();
	};
	virtual void quit() = 0;

private:
	virtual void physicsUpdate() = 0;
	virtual void inputHandle() = 0;
	virtual void update() = 0;
	virtual void renderScene() = 0;
};

