#pragma once

#include "GameEvent.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "InputHandler.h"

#include "Helpers.h"

class GameManager 
{
	using ObjectComponents = Vector<u_ptr<Component>>;
	using ComponentsContainer = Vector<u_ptr<ObjectComponents>>;
	
public:
	GameManager() = default;
	~GameManager();

	int getScore() const;
	int getLives() const;
	GameEvent getLastEvent() const;
	Vector<GameObject>& getGameObjects();
	ComponentsContainer& getComponentsContainer();
	void setLastEvent(const GameEvent gameEvent);
	void setScore(int score);
	void reduceLife();

	void reset();

	GameObject createGameObject();
	void addComponentToObject(GameObject& gameObject, Component* component) const;
	ObjectComponents* setComponentsForNewObject();

private:
	int _score = 0;
	int _lives = 3;
	GameEvent _lastEvent = GameEvent::NONE; 
	Vector<GameObject> _gameObjects {};
	ComponentsContainer _componentsContainer {};
};