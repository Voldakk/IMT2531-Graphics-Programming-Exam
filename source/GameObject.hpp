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
	std::shared_ptr<Transform> transform;
	std::vector<std::shared_ptr<Component>> components;

	GameObject();

	void Update(float deltaTime);

	template <class T>
	std::shared_ptr<T> AddComponent();

	void SetParent(GameObject * newParent);
	void SetParent(std::shared_ptr<GameObject>& newParent);

	int GetChildIndex(Transform * child) const;
	int GetChildIndex(GameObject * child) const;
};

template<class T>
std::shared_ptr<T> GameObject::AddComponent()
{
	std::shared_ptr<T> component = std::make_shared<T>(this);
	components.push_back(component);

	return component;
}