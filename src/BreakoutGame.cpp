#include "BreakoutGame.h"

template <typename T>
std::optional<T*> GameObject::getComponentPtr(ComponentType type) const
{
	if (std::optional<Component*> componentOpt = getComponentPtr(type); componentOpt.has_value())
		try
	{
		return std::optional<T*>(dynamic_cast<T*>(componentOpt.value()));
	}
	catch (const std::bad_cast& e)
	{
		std::cout << "Bad component cast! Casting from Component* to specific type\n" << e.what();
	}
	return {};
}


BreakoutGame::BreakoutGame(Window& gameWindow, GameManager& gameManager, InputHandler& inputHandler, Level& level, SoundManager& soundManager)
: _gameWindow(gameWindow), _gameManager(gameManager), _inputHandler(inputHandler), _level(level), _soundManager(soundManager){}

GameObject& BreakoutGame::getObjectByTag(String tag) const
{
	for (auto& obj : _gameManager.getGameObjects())
		if (obj.getTag() == tag)
			return obj;

	throw std::exception(std::format("No objects found with tag: {}", tag).c_str());
}

bool BreakoutGame::isLifeLost() const
{
	Transform const* ballTransform = nullptr;
	Transform const* playerTransform = nullptr;

	for (auto const& obj : _gameManager.getGameObjects())
		if (obj.getTag() == "player") 
			playerTransform = obj.getComponentPtr<Transform>(ComponentType::TRANSFORM).value_or(nullptr);
		else 
			if (obj.getTag() == "ball") 
				ballTransform = obj.getComponentPtr<Transform>(ComponentType::TRANSFORM).value_or(nullptr);

	if (ballTransform && playerTransform)
	{
		return ballTransform->getY() > playerTransform->getY();
	}

	return false;
}

bool BreakoutGame::areAllBricksDestroyed() const
{
	for (auto const& object : _gameManager.getGameObjects())
	{
		if (object.getTag() == "brick" && any_cast<char>(object.getProperty("brickType")) != 'I')
			return false;
	}
	return true;
}

void BreakoutGame::initialize()
{
	_gameManager.reset();
	_level.initializeBricks(_gameManager, _gameWindow);
	_level.initializePlayer(_gameManager, _gameWindow);
	_level.initializeBall(_gameManager, _gameWindow);
}

void BreakoutGame::quit()
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void BreakoutGame::physicsUpdate()
{
	checkBallWindowCollision();

	for (auto& gameObject : _gameManager.getGameObjects())
	{
		gameObject.applyForce();
	}

	GameObject& ball = getObjectByTag("ball");

	auto it = _gameManager.getGameObjects().begin();
	for (auto& object : _gameManager.getGameObjects())
	{
		if (object.getTag() == "brick" || object.getTag() == "player")
		{
			auto [collision, invertXY] =
				ball.getComponentPtr<CircleCollider>(ComponentType::COLLIDER).value()->
				collidesWith(
					object.getComponentPtr<BoxCollider>(ComponentType::COLLIDER).value());

			if (collision) {
				ball.applyForce(-1);

				auto forceOpt = ball.getComponentPtr<Force>(ComponentType::FORCE);
				if (invertXY.first) forceOpt.value()->invertY();
				if (invertXY.second) forceOpt.value()->invertX();

				if (object.getTag() == "brick")
				{
					int health = std::any_cast<int>(object.getProperty("health"));
					if (health == 1)
					{

						if (auto soundPath = _level.getBrickType(any_cast<char>(object.getProperty("brickType"))); 
							soundPath.has_value() && !soundPath.value().getBreakSound().empty()) 
							_soundManager.play(_soundManager.load(soundPath.value().getBreakSound().string()));

						_gameManager.setScore(_gameManager.getScore() + std::any_cast<int>(object.getProperty("points")));
						_gameManager.getGameObjects().erase(it);
					}
					else
					{
						if (auto soundPath = _level.getBrickType(any_cast<char>(object.getProperty("brickType")));
							soundPath.has_value() && !soundPath.value().getHitSound().empty())
							_soundManager.play(_soundManager.load(soundPath.value().getHitSound().string()));

						if (health != -1)
						{
							--health;
							std::any value{ health };
							object.setProperty("health", value);
						}
					}
				}
				break;
			}
		}
		it++;
	}
}

void BreakoutGame::inputHandle()
{
	InputHandler::update();
}

void BreakoutGame::update()
{
	using enum InputKey;
    auto keyPressed = InputHandler::getKeyPressed();

	auto ball = getObjectByTag("ball");

	if (keyPressed == LEFT || keyPressed == RIGHT)
	{
		int moveValue = (keyPressed == LEFT) ? -1 : 1;
		int speed = 20;
		for (GameObject& object : _gameManager.getGameObjects()) {
			if (object.getTag() == "player")
			{
				object.move(moveValue * speed, 0);
				auto transformOpt = object.getComponentPtr<Transform>(ComponentType::TRANSFORM);
				if (Transform* transform = transformOpt.value_or(nullptr))
				{
					int x = transform->getX();
					int xMin = 0; int xMax = _gameWindow.getWidth() - transform->getW();
					x = std::min(std::max(xMin, x), xMax);
					transform->setX(x);
				}
			}
		}
	}

	if (isLifeLost())
	{
		if (_gameManager.getLives() < 1)
			_gameManager.setLastEvent(GameEvent::LEVEL_RESET);
		else
		{
			_gameManager.setLastEvent(GameEvent::LIFE_LOST);
			resetPlayerPosition();
			resetBallPosition();
			_gameManager.reduceLife();
		}
	}

	if (areAllBricksDestroyed() || keyPressed == NEXT)
	{
		_gameManager.setLastEvent(GameEvent::NEXT_LEVEL);
	}

	if (keyPressed == QUIT) _gameManager.setLastEvent(GameEvent::EXIT);
}

void BreakoutGame::renderScene()
{
	_gameWindow.renderFrame(_gameManager.getGameObjects(), _gameManager.getScore());
}

void BreakoutGame::checkBallWindowCollision() const
{
	GameObject ball = getObjectByTag("ball");

	auto transformOpt = ball.getComponentPtr<Transform>(ComponentType::TRANSFORM);
	auto const* transform = transformOpt.value();

	int x = transform->getX();
	int y = transform->getY();
	int w = transform->getW();
	int h = transform->getH();

	if (0 > x || x + w > 800) {
		ball.applyForce(-1);
		ball.getComponentPtr<Force>(ComponentType::FORCE).value()->invertX();
	}
	if (0 > y || y + h > 600)
	{
		ball.applyForce(-1);
		ball.getComponentPtr<Force>(ComponentType::FORCE).value()->invertY();
	}
}

void BreakoutGame::resetBallPosition() const
{
	_level.resetBall(getObjectByTag("ball"));
}

void BreakoutGame::resetPlayerPosition() const
{
	_level.resetPlayer(getObjectByTag("player"));
}

