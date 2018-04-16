#pragma once

#include "EVA.hpp"

#include "Pacman.hpp"
#include "TileMap.hpp"
#include "Ghost.hpp"

class Game : public EVA::Scene
{
	struct Wave
	{
		GhostState state;
		float time;
	};
	std::vector<Wave> m_Waves;
	int m_CurrentWave;
	float m_WaveTimer;

	unsigned int m_Score;

public:

	TileMap * tileMap;
	Pacman* pacman;
	std::vector<Ghost*> ghosts;

	/**
     * \brief Setups the game scene
     */
    Game();

	/**
	* \brief Runs every frame
	* \param deltaTime The time in seconds between frames
	*/
    void Update(float deltaTime) override;

	/**
	 * \brief Gets the current wave
	 * \return The current wave
	 */
	Wave CurrentWave() { return m_Waves[m_CurrentWave]; }

	/**
	 * \brief Adds score to the player
	 * \param amount The amount to add
	 */
	void AddScore(unsigned int amount);
};