#pragma once

#include <vector>
#include <memory>

#include "Component.hpp"
#include "Transform.hpp"

class Scene;

class GameObject
{
public:

	Scene * scene{};
	std::unique_ptr<Transform> transform;
	std::vector<std::shared_ptr<Component>> components;

	GameObject();

	void Update(float deltaTime);
	void Render();

	template <class T>
	std::shared_ptr<T> AddComponent();
};

template<class T>
std::shared_ptr<T> GameObject::AddComponent()
{
	std::shared_ptr<T> component = std::make_shared<T>(this);
	components.push_back(component);

	return component;
}