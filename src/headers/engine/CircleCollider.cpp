#include "CircleCollider.h"

CircleCollider::CircleCollider(Transform* transform, int radius) : Collider(transform, ColliderType::CIRCLE), _radius(radius) 
{
    setMoveable(true); 
}

int CircleCollider::getRadius() const { return _radius; };
void CircleCollider::setRadius(int radius) { _radius = radius; };

std::pair<bool, std::pair<bool, bool>> CircleCollider::collidesWith(Collider<ColliderType::BOX>* other) 
{
    BoxCollider const* boxCollider = nullptr;
    try
    {
        boxCollider = dynamic_cast<BoxCollider*>(other);
    }
    catch (const std::bad_cast& e)
    {
        std::cout << std::format("Collider cast exception!\n {}", e.what());
        return { false, {}};
    }

    auto [circleCenterX, circleCenterY] = getCenter();
    auto circleRadius = getRadius();

    auto [boxCenterX, boxCenterY] = boxCollider->getCenter();
    auto [boxWidth, boxHeight] = boxCollider->getWidthHeight();

    int deltaX = abs(circleCenterX - boxCenterX);
    int deltaY = abs(circleCenterY - boxCenterY);

    bool horizontalCollision = (static_cast<float>(boxWidth) / boxHeight) >= (static_cast<float>(deltaX) / deltaY);
    bool verticalCollision = (static_cast<float>(boxWidth) / boxHeight) <= (static_cast<float>(deltaX) / deltaY);

    return { ((deltaX < circleRadius + boxWidth / 2) && (deltaY < circleRadius + boxHeight / 2)) , {horizontalCollision, verticalCollision} };
}

std::pair<bool, std::pair<bool, bool>> CircleCollider::collidesWith(Collider<ColliderType::CIRCLE>* other)
{
    return {};
}

void CircleCollider::update() {}

