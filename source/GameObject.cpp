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

void GameObject::SetParent(GameObject* newParent)
{
	transform->SetParent(newParent->transform);
}

void GameObject::SetParent(std::shared_ptr<GameObject>& newParent)
{
	transform->SetParent(newParent->transform);
}


int GameObject::GetChildIndex(Transform* child) const
{
	return transform->GetChildIndex(child);
}

int GameObject::GetChildIndex(GameObject* child) const
{
	return transform->GetChildIndex(child);
}
