#include "GameManager.h"

GameManager::~GameManager()
{
	_gameObjects.clear();

	for (auto i = 0; i < _componentsContainer.size(); ++i)
	{
		auto& objComps = _componentsContainer.at(i);
		for (auto j = 0; j < objComps->size(); ++j)
		{
			auto& comp = objComps->at(j);
			comp.release()->~Component(); // sorry you had to see this, but reset() did not work...
		}
		objComps.reset();
	}
}

int GameManager::getScore() const{ return _score; }
void GameManager::setScore(int score) { _score = score; }

GameEvent GameManager::getLastEvent() const { return _lastEvent; }
Vector<GameObject>& GameManager::getGameObjects() { return _gameObjects; }
GameManager::ComponentsContainer& GameManager::getComponentsContainer() { return _componentsContainer; }

void GameManager::setLastEvent(const GameEvent gameEvent) { _lastEvent = gameEvent; }

GameObject GameManager::createGameObject() 
{
	return GameObject(setComponentsForNewObject());
}

void GameManager::addComponentToObject(GameObject& gameObject, Component* component ) const
{
	gameObject.getComponentsPtr()->push_back(u_ptr<Component>(component));
}

GameManager::ObjectComponents* GameManager::setComponentsForNewObject()
{
	using namespace std;

	_componentsContainer.push_back(make_unique<Vector<u_ptr<Component>>>());
	_componentsContainer.back()->push_back(make_unique<Transform>());

	return _componentsContainer.back().get();
}

void GameManager::reset()
{
	_score = 0;
	_lives = 3;
	_lastEvent = GameEvent::NONE;
	_gameObjects.clear();
}

void GameManager::reduceLife()
{
	_lives--;
}

int GameManager::getLives() const
{
	return _lives;
}

