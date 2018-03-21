#include "GameObject.hpp"

#include "Scene.hpp"

GameObject::GameObject()
{
	transform = std::make_unique<Transform>();
	transform->gameObject = this;
}

void GameObject::Update(float deltaTime)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Update(deltaTime);
	}
}

void GameObject::Render()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Render();
	}
}