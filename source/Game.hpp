#pragma once

#include <vector>
#include <memory>

#include "Scene.hpp"
#include "GameObject.hpp"

class Game : public Scene
{
public:

	Game();

	/// <summary>Called every frame.</summary>
	/// <param name="deltaTime">The time in seconds between frames</param>
	void Update(float deltaTime) override;

	/// <summary>Render all game elements</summary>
	void Render() override;

private:

	std::vector<std::shared_ptr<GameObject>> gameObjects;
};

