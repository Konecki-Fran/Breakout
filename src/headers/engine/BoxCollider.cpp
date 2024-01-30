#include "BoxCollider.h"

BoxCollider::BoxCollider(Transform* transform, int w, int h) : Collider(transform, ColliderType::BOX), _w(w), _h(h) 
{ 
	setMoveable(true); 
}

std::pair<bool, std::pair<bool, bool>> BoxCollider::collidesWith(Collider<ColliderType::BOX>* other)
{
	return {};
}

std::pair<bool, std::pair<bool, bool>> BoxCollider::collidesWith(Collider<ColliderType::CIRCLE>* other)
{
	return other->collidesWith(this);
}

std::pair<int, int> BoxCollider::getWidthHeight() const 
{ 
	return { _w, _h }; 
}

void BoxCollider::setWidthHeight(std::pair<int, int> widthHeight) 
{ 
	_w = widthHeight.first;
	_h = widthHeight.second;
}

void BoxCollider::update() {}

