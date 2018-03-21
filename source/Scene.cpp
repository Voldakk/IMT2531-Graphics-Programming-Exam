#include "Scene.hpp"

#include <memory>

void Scene::Update(const float deltaTime)
{
	for (auto& gameObject : gameObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void Scene::Render()
{
	if (skybox != nullptr)
	{
		skybox->Render();
	}

	for (auto& gameObject : gameObjects)
	{
		gameObject->Render();
	}
}

std::shared_ptr<GameObject> Scene::CreateGameObject()
{
	auto gameObject = std::make_shared<GameObject>();

	gameObject->scene = this;

	gameObjects.push_back(gameObject);

	return gameObject;
}
