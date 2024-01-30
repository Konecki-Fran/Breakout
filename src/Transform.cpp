#include "Transform.h"

Transform::Transform() : Component(ComponentType::TRANSFORM) { setMoveable(true); }

int Transform::getX() const { return _x; }
int Transform::getY() const { return _y; }
int Transform::getH() const { return _h; }
int Transform::getW() const { return _w; }
void Transform::setX(int x) { _x = x; }
void Transform::setY(int y) { _y = y; }
void Transform::setH(int h) { _h = h; }
void Transform::setW(int w) { _w = w; }
void Transform::set(int x, int y, int w, int h) { setX(x); setY(y); setW(w); setH(h); }
void Transform::set(int x, int y) { setX(x); setY(y); }
void Transform::set(Vector4<int> values) { setX(values.x); setY(values.y); setW(values.z); setH(values.w); }

