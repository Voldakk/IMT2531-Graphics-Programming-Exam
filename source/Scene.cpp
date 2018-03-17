#pragma once

#include <memory>

#include "Scene.hpp"

void Scene::Update(float deltaTime)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update(deltaTime);
	}
}

void Scene::Render()
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Render();
	}
}

std::shared_ptr<GameObject> Scene::CreateGameObject()
{
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

	gameObject->scene = this;

	gameObjects.push_back(gameObject);

	return gameObject;
}
