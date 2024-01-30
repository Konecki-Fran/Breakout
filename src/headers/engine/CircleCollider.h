#pragma once

#include "Collider.h"
#include "BoxCollider.h"
#include "ColliderType.h"

#include <math.h>
#include <memory>
#include <iostream>

class CircleCollider : public Collider<ColliderType::CIRCLE> 
{
public:
	explicit CircleCollider(Transform* transform, int radius);
	~CircleCollider() final = default;

	int getRadius() const;
	void setRadius(int radius);

	std::pair<bool, std::pair<bool, bool>> collidesWith(Collider<ColliderType::BOX>* other) override;
	std::pair<bool, std::pair<bool, bool>> collidesWith(Collider<ColliderType::CIRCLE>* other) override;

	void update() override;

private:
	int _radius{};
};