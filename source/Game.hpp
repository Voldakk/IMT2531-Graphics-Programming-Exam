#pragma once

#include "EVA.hpp"

#include "Pacman.hpp"
#include "TileMap.hpp"

class Game : public EVA::Scene
{
public:

	std::shared_ptr<Pacman> pacman;
	std::shared_ptr<TileMap> tileMap;


    Game();

    /// <summary>Called every frame.</summary>
    /// <param name="deltaTime">The time in seconds between frames</param>
    void Update(float deltaTime) override;
};
