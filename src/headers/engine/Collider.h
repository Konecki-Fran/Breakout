#pragma once

#include "Component.h"
#include "ComponentType.h"
#include "ColliderType.h"
#include "Moveable.h"
#include "Transform.h"

#include <utility>

template <ColliderType T>
class Collider : public Component, public Moveable
{
public:
	explicit Collider(Transform* transform, ColliderType colliderType) 
		: Component(ComponentType::COLLIDER), _transform(transform), _colliderType(colliderType)
	{
		setMoveable(true);
	};

	virtual ~Collider() = default;

	virtual std::pair<bool, std::pair<bool, bool>> collidesWith(Collider<ColliderType::BOX>* other) = 0;
	virtual std::pair<bool, std::pair<bool, bool>> collidesWith(Collider<ColliderType::CIRCLE>* other) = 0;

	int getX() const override { return _transform->getX() + _transform->getW() / 2; };
	int getY() const override { return _transform->getY() + _transform->getH()/2; };
	std::pair<int, int> getCenter() const { return { getX(), getY()}; };
	ColliderType getColliderType() const { return _colliderType; };
	void setX(int x) override { };
	void setY(int y) override {  };
	void setCenter([[maybe_unused]] std::pair<int, int> center) const {  };
	void setColliderType(ColliderType colliderType) { _colliderType = colliderType; };

private:
	Transform* _transform;
	ColliderType _colliderType;
};

