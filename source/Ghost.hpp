#pragma once

#include "EVA.hpp"
#include "EVA/Graphics.hpp"

class Game;
class TileMap;
class Pacman;

/// <summary>Different ghost behaviours</summary>
enum GhostState
{
	/// <summary>The ghost's standard behaviours</summary>
	Chase,
	/// <summary>Scatter to a predefined area</summary>
	Scatter,
	/// <summary>Move randomly. Can be eaten by Pacman</summary>
	Frightened,
	/// <summary>Dead. Should go home to respawn</summary>
	Dead
};

class Ghost : public EVA::Component
{
public:

	enum Direction { Up, Down, Right, Left };

private:

	// Model
	std::shared_ptr<EVA::Model> m_Model;
	std::shared_ptr<EVA::Material> m_BodyMaterial;
	std::shared_ptr<EVA::Material> m_EyeMaterial;

	// Movement
	glm::ivec2 m_TargetTile;
	Direction m_Direction;
	glm::ivec2 m_CurrentDirection;

	float m_MovementSpeed = 2.0f;

	// The ghosts current state
	GhostState m_State;

protected: 

	glm::ivec2 m_CurrentTile;

	// Other game elements
	Game* m_Game;
	std::shared_ptr<TileMap> m_TileMap;
	std::shared_ptr<Pacman> m_Pacman;

	// The tile the ghost targets while in the scatter state
	glm::ivec2 m_ScatterTile;

public:

	glm::ivec2& currentTile = m_CurrentTile;

	/**
	 * \brief Constructor. Loads the model and setsup the materials. Calls Reset()
	 * \param gameObject The GameObject the ghost is attaced to
	 * \param game The game scene
	 */
	explicit Ghost(EVA::GameObject* gameObject, Game* game);

	/**
	 * \brief Runs every frame
	 * \param deltaTime The time in seconds between frames
	 */
	void Update(float deltaTime) override;

	/**
	 * \brief Resets the ghost to it's original state
	 */
	void Reset();

	/**
	 * \brief Sets the ghost's state
	 * \param newState The new state
	 */
	void SetState(GhostState newState);

private:

	/**
	 * \brief Translates a Direction to a ivec2 pointing in the given direction
	 * \param direction The direction
	 * \return A ivec2 pointing in the given direction
	 */
	static glm::ivec2 DirectionToVector(Direction direction);

	/**
	 * \brief Translates a glm::ivec2 to a Direction
	 * \param direction The ivec2
	 * \return The Direction the ivec2 is pointing to
	 */
	static Direction VectorToDirection(glm::ivec2 direction);

	/**
	 * \brief Finds all the possible directions from the current tile
	 * \return A list of all the possible directions
	 */
	std::vector<Direction> FindPossibleDirections() const;

	/**
	 * \brief Chooses a driection based on the ghosts's state and the current tile it's standing on
	 * \param directions A list of directions to choose form
	 * \return The choosen direction
	 */
	Direction ChooseDirection(const std::vector<Ghost::Direction>& directions) const;

protected:

	/**
	 * \brief Finds the "fastest" directiopn to the given target
	 * \param directions A list of directions to choose form
	 * \param target The target
	 * \return The "fastest" direction
	 */
	Direction DirectionToTarget(const std::vector<Ghost::Direction>& directions, glm::ivec2 target) const;

	/**
	 * \brief Chooses the target when the ghost is in it's chase state on a normal floor tile
	 * \param directions  A list of directions to choose form
	 * \return The choosen direction
	 */
	virtual Direction ChooseChaseTarget(const std::vector<Ghost::Direction>& directions) const = 0;

	/**
	 * \brief Sets the ghost's material color
	 * \param newColor The new color
	 */
	void SetColor(glm::vec3 newColor) const;
};