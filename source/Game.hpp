#pragma once

#include "EVA.hpp"

#include "Pacman.hpp"
#include "TileMap.hpp"
#include "Ghost.hpp"

class Game : public EVA::Scene
{
public:

	std::shared_ptr<Pacman> pacman;
	std::shared_ptr<TileMap> tileMap;

	std::vector<std::shared_ptr<Ghost>> ghosts;


	/**
     * \brief Setups the game scene
     */
    Game();

	/**
	* \brief Runs every frame
	* \param deltaTime The time in seconds between frames
	*/
    void Update(float deltaTime) override;
};
