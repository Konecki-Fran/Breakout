#pragma once

#include "EngineComponents.h"
#include "Helpers.h"

#include <type_traits>
#include <any>
#include <unordered_map>

class GameObject 
{
public:
	explicit GameObject(Vector<u_ptr<Component>>* components);
	~GameObject() = default;

	String getTag() const;
	std::any getProperty(StringView key) const;
	void setTag(StringView tag);
	void setProperty(StringView key, std::any value);
		
	Vector<u_ptr<Component>>* getComponentsPtr() const { return _components; };
	std::optional<Component*> getComponentPtr(ComponentType type) const;
	template <typename T>
	std::optional<T*> getComponentPtr(ComponentType type) const;
	
	template<> std::optional<Force*> getComponentPtr<Force>(ComponentType type) const;
	template<> std::optional<Transform*> getComponentPtr<Transform>(ComponentType type) const;
	template<> std::optional<BoxCollider*> getComponentPtr<BoxCollider>(ComponentType type) const;
	template<> std::optional<CircleCollider*> getComponentPtr<CircleCollider>(ComponentType type) const;
	
	void applyForce(int direction = 1);
	void move(int x, int y);
	void update();

private:
	unsigned long _id {};
	Vector<u_ptr<Component>>* _components;
	String _tag {};
	std::unordered_map<String, std::any> _properties{};
};