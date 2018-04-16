#pragma once

#include "EVA.hpp"

class Game;

class Pellet : public EVA::Component
{

	static std::shared_ptr<EVA::Mesh> m_Mesh;
	static std::shared_ptr<EVA::Material> m_Material;

	glm::ivec2 m_Tile;

protected:

	Game* m_Game;
	unsigned int m_Score = 10;

public:

	/**
	 * \brief Constructor. Adds a MeshRenderer and positions the pellet
	 * \param gameObject The GameObject the pellet is attaced to
	 * \param game The game scene
	 * \param tile The tile index where the pellet is located
	 */
	explicit Pellet(EVA::GameObject* gameObject, Game* game, glm::ivec2 tile);

	/**
	* \brief Runs every frame
	* \param deltaTime The time in seconds between frames
	*/
	void Update(float deltaTime) override;

	/**
	 * \brief Called when the pellet is picked up
	 */
	virtual void OnPickup();

	/**
	 * \brief Loads the pellet mesh and material
	 */
	static void Init();
};

class Energizer : public Pellet
{
	unsigned int m_Score = 50;
	float time = 8.0f;

public:
	Energizer(EVA::GameObject* gameObject, Game* game, const glm::ivec2& tile)
		: Pellet(gameObject, game, tile)
	{
		Pellet::m_Score = m_Score;
	}

	void OnPickup() override;
};
