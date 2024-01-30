#pragma once

#include "ComponentType.h"

class Component 
{
public:
	explicit Component(ComponentType type);
	virtual ~Component() = default;

	ComponentType getType() const;
	bool isMoveable() const;

	void setMoveable(bool moveable);

	virtual void update();
private:
	bool _moveable = false;
	ComponentType _type;
};