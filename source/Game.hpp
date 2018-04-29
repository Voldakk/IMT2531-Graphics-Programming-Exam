#pragma once

//#include "EVA.hpp"
#include "EVA/UI.hpp"

#include "../EVA/Components/FreeLook.hpp"
#include "../EVA/Components/FollowTarget.hpp"

#include "Pacman.hpp"
#include "TileMap.hpp"
#include "Ghost.hpp"

class Game : public EVA::Scene
{
	static const unsigned int EXTRA_LIVES = 2;

	struct Wave
	{
		GhostState state;
		float time;
	};
	std::vector<Wave> m_Waves;
	unsigned int m_CurrentWave = 0;
	float m_WaveTimer = 0.0f;

	unsigned int m_Score = 0;
	unsigned int m_ExtraLives = 2;

	// Energizer
	bool m_ActiveEnergizer = false;
	float m_EnergizerTimer = 0.0f;
	float m_Time = 0.0f;

	// UI
	EVA::Label* m_ScoreLabel;
	EVA::Label* m_ExtraLivesLabel;
	EVA::Label* m_CameraModeLabel;

	// Pause menu
	bool m_IsPaused = false;

	// Camera
	enum CameraMode { TopDown, Chase, Free, Last };
	unsigned int m_CurrentCameraMode;

	EVA::Camera* m_Camera;
	EVA::FreeLook* m_FreeCamera;
	EVA::FollowTarget* m_ChaseCamera;

public:

	TileMap* tileMap;
	Pacman* pacman;
	std::vector<Ghost*> ghosts;

	/**
     * \brief Setups the game scene
     */
	explicit Game();

	/**
	* \brief Runs every frame
	* \param deltaTime The time in seconds between frames
	*/
    void Update(float deltaTime) override;

	/**
	 * \brief Gets the current wave
	 * \return The current wave
	 */
	Wave CurrentWave();

	/**
	 * \brief Adds score to the player
	 * \param amount The amount to add
	 */
	void AddScore(unsigned int amount);

	/**
	 * \brief 
	 */
	void ActivateEnergizer(float time);

	void Unpause();

	void Reset();

	void UpdateCameraMode();

	void Win();
};