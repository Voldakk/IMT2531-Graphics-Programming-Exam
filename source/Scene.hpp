#pragma once

#include <vector>
#include <memory>

#include "Light.hpp"
#include "GameObject.hpp"

// A scene that can be run by the SceneManager
class Scene
{
public:

	/// <summary>Shared pointer to itself</summary>
	std::shared_ptr<Scene> self;

	/// <summary></summary>
	Light light;

	/// <summary></summary>
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	/// <summary>Allows the user to close the menu by pressing escape</summary>
	/// <param name="deltaTime">The time in seconds between frames</param>
    virtual void Update(float deltaTime);

	/// <summary>Render all scene elements to the screen</summary>
    virtual void Render();

	/// <summary></summary>
	std::shared_ptr<GameObject> CreateGameObject();
};