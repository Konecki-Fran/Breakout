#pragma once

#include "Transform.h"

class Force : public Component
{
public:
	explicit Force(int xMove, int yMove);
	~Force() final = default;

	int getXMove() const;
	int getYMove() const;

	void invertX();
	void invertY();
	void update() override;

private:
	int _xMove;
	int _yMove;
};