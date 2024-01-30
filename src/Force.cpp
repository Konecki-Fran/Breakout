#include "Force.h"

Force::Force(int xMove, int yMove)
	: Component(ComponentType::FORCE), _xMove(xMove), _yMove(yMove) {};


int Force::getXMove() const { return _xMove; }
int Force::getYMove() const { return _yMove; }

void Force::invertX() { _xMove = -_xMove;}
void Force::invertY() { _yMove = -_yMove; }

void Force::update() { }
