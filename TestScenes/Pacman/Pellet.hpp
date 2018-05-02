#pragma once

#include "EVA.hpp"

class Game;

class Pellet : public EVA::Component, public EVA::IUpdateComponent
{

	static std::shared_ptr<EVA::Mesh> m_Mesh;
	static std::shared_ptr<EVA::Material> m_Material;

	static unsigned int m_PelletCount;

protected:

	unsigned int m_Score = 10;

public:

	Game* game;
	glm::ivec2 tile;

	void Start() override;

	/**
	* \brief Runs every frame
	* \param deltaTime The time in seconds between frames
	*/
	void Update(float deltaTime) override;

	/**
	 * \brief Called when the pellet is picked up
	 */
	virtual void OnPickup();

	static void ResetCount();
};

class Energizer : public Pellet
{
	unsigned int m_Score = 50;
	float m_Time = 8.0f;

public:

	void Start() override;

	void OnPickup() override;
};
