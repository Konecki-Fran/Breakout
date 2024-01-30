#include "Component.h"

Component::Component(ComponentType type) : _type(type) {};

ComponentType Component::getType() const { return _type; }

bool Component::isMoveable() const
{
	return _moveable;
}

void Component::setMoveable(bool moveable) 
{
	_moveable = moveable;
}


void Component::update() {};
