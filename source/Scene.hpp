#pragma once
#include <memory>

#include "Light.hpp"

// A scene that can be run by the SceneManager
class Scene
{
public:
	/// <summary>Allows the user to close the menu by pressing escape</summary>
	/// <param name="deltaTime">The time in seconds between frames</param>
    virtual void Update(float deltaTime){};

	/// <summary>Render all scene elements to the screen</summary>
    virtual void Render(){};

	/// <summary>Shared pointer to itself</summary>
	std::shared_ptr<Scene> self;

	Light light;
};