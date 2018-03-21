#include "GameObject.hpp"

#include "Scene.hpp"

GameObject::GameObject()
{
	transform = std::make_unique<Transform>(this);
}

void GameObject::Update(const float deltaTime)
{
	for (auto& component : components)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Render()
{
	for (auto& component : components)
	{
		component->Render();
	}
}