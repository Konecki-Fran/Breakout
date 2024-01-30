#pragma once

#include <iostream>

class Moveable
{
public:
	virtual ~Moveable() = default;

	void move(int x, int y);

	virtual int getX() const = 0;
	virtual int getY() const = 0;
	virtual void setX(int x) = 0;
	virtual void setY(int y) = 0;
	
};