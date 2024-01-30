#include "GameObject.h"

GameObject::GameObject(Vector<u_ptr<Component>>* components) : _components(components) {};

String GameObject::getTag() const { return _tag; }
void GameObject::setTag(StringView tag) { _tag = tag; }

std::any GameObject::getProperty(StringView key) const { return _properties.at(key.data()); }
void GameObject::setProperty(StringView key, std::any value) 
{ 
    _properties.insert_or_assign(key.data(), value); 
};

std::optional<Component*> GameObject::getComponentPtr(ComponentType type) const
{
	std::optional<Component*> result{};

	for (const auto& componentPtr : (*_components))
		if (componentPtr->getType() == type)
		{
			result.emplace(componentPtr.get());
		}

	return result;
}

template <typename T>
std::optional<T*> GameObject::getComponentPtr(ComponentType type) const
{
    if (std::optional<Component*> componentOpt = getComponentPtr(type); componentOpt.has_value())
        try
    {
        return std::optional<T*>(dynamic_cast<T*>(componentOpt.value()));
    }
    catch (const std::bad_cast& e)
    {
        std::cout << "Bad component cast! Casting from Component* to specific type\n" << e.what();
    }
    return {};
}

void GameObject::move(int dx, int dy)
{
    auto* components = getComponentsPtr();
    auto i = components->size() - 1;

    while (i--)
        if (components->at(i)->isMoveable())
            dynamic_cast<Moveable*>(components->at(i).get())->move(dx, dy);
}

void GameObject::applyForce(int direction)
{
    auto forceOpt = getComponentPtr<Force>(ComponentType::FORCE);
    if (forceOpt.has_value())
    {
        int dx = direction * forceOpt.value()->getXMove();
        int dy = direction * forceOpt.value()->getYMove();
        move(dx, dy);   
    }
}






template <>
std::optional<Force*> GameObject::getComponentPtr<Force>(ComponentType type) const
{
    if (std::optional<Component*> componentOpt = getComponentPtr(type); componentOpt.has_value())
        try
    {
        return std::optional<Force*>(dynamic_cast<Force*>(componentOpt.value()));
    }
    catch (const std::bad_cast& e)
    {
        std::cout << "Bad component cast! Casting from Component* to specific type\n" << e.what();
    }
    return {};
}

template <>
std::optional<Transform*> GameObject::getComponentPtr<Transform>(ComponentType type) const
{
    if (std::optional<Component*> componentOpt = getComponentPtr(type); componentOpt.has_value())
        try
    {
        return std::optional<Transform*>(dynamic_cast<Transform*>(componentOpt.value()));
    }
    catch (const std::bad_cast& e)
    {
        std::cout << "Bad component cast! Casting from Component* to specific type\n" << e.what();
    }
    return {};
}

template <>
std::optional<BoxCollider*> GameObject::getComponentPtr<BoxCollider>(ComponentType type) const
{
    if (std::optional<Component*> componentOpt = getComponentPtr(type); componentOpt.has_value())
        try
    {
        return std::optional<BoxCollider*>(dynamic_cast<BoxCollider*>(componentOpt.value()));
    }
    catch (const std::bad_cast& e)
    {
        std::cout << "Bad component cast! Casting from Component* to specific type\n" << e.what();
    }
    return {};
}

template <>
std::optional<CircleCollider*> GameObject::getComponentPtr<CircleCollider>(ComponentType type) const
{
    if (std::optional<Component*> componentOpt = getComponentPtr(type); componentOpt.has_value())
        try
    {
        return std::optional<CircleCollider*>(dynamic_cast<CircleCollider*>(componentOpt.value()));
    }
    catch (const std::bad_cast& e)
    {
        std::cout << "Bad component cast! Casting from Component* to specific type\n" << e.what();
    }
    return {};
}

