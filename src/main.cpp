#include "Window.h"
#include "InputHandler.h"
#include "Level.h"
#include "Force.h"

#include "EngineComponents.h"
#include "BreakoutGame.h"

#include "Helpers.h"

#include <chrono>
#include <thread>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

InputKey InputHandler::_keyPressed{ InputKey::NONE };

void slowDown()
{
	using namespace std;
	this_thread::sleep_for(chrono::milliseconds(10));
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	auto window = Window(WINDOW_WIDTH, WINDOW_HEIGHT);
	auto input = InputHandler();
	auto manager = GameManager();
	int levelNumber = 1;
	auto level = Level(levelNumber);
	auto soundManager = SoundManager();

	auto game = BreakoutGame(window, manager, input, level, soundManager);

	game.initialize();

	bool running = true;
	while (running)
	{
		using enum GameEvent;
		auto lastEvent = manager.getLastEvent();
		if (lastEvent == EXIT) running = false;
		if (lastEvent == NEXT_LEVEL || lastEvent == LEVEL_RESET)
		{
			if (lastEvent == NEXT_LEVEL) 
				levelNumber = (levelNumber % 3) + 1;

			level = Level(levelNumber);
			manager.reset();
			game.initialize();
		}

		game.updateFrame();
		slowDown();
	}

	game.quit();

	return 0;
}