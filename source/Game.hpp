#pragma once

#include "EVA-Engine/Scene.hpp"

class Game : public Scene
{
public:

	Game();

	/// <summary>Called every frame.</summary>
	/// <param name="deltaTime">The time in seconds between frames</param>
	void Update(float deltaTime) override;
};
