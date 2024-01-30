#pragma once

#include "Collider.h"
#include "ColliderType.h"

class BoxCollider : public Collider<ColliderType::BOX> {
public:
	explicit BoxCollider(Transform* transform, int w, int h);
	~BoxCollider() final = default;

	std::pair<int, int> getWidthHeight() const;
	void setWidthHeight(std::pair<int, int> widthHeight);

	std::pair<bool, std::pair<bool, bool>> collidesWith(Collider<ColliderType::BOX>* other) override;
	std::pair<bool, std::pair<bool, bool>> collidesWith(Collider<ColliderType::CIRCLE>* other) override;

	void update() override;

private:
	int _w{};
	int _h{};
};