#pragma once

#include "Scene.hpp"
#include "Cube.hpp"

class Game : public Scene
{
public:
	Game();
	~Game();

	/// <summary>Called every frame.</summary>
	/// <param name="deltaTime">The time in seconds between frames</param>
	void Update(float deltaTime) override;

	/// <summary>Render all game elements</summary>
	void Render() override;

private:
	Primitive * cube;
	Primitive * cube2;
};

