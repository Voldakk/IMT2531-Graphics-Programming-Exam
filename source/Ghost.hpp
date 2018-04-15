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
	enum Direction { Up, Down, Right, Left };

	// Model
	std::shared_ptr<EVA::Model> m_Model;
	std::shared_ptr<EVA::Material> m_BodyMaterial;
	std::shared_ptr<EVA::Material> m_EyeMaterial;

	// Other game elements
	std::shared_ptr<TileMap> m_TileMap;
	std::shared_ptr<Pacman> m_Pacman;

	//
	GhostState m_State;
	glm::ivec2 m_ScatterTile;

	glm::ivec2 m_CurrentTile;
	glm::ivec2 m_TargetTile;

	Direction m_Direction;
	glm::ivec2 m_CurrentDirection;

	float m_MovementSpeed = 2.0f;

public:

	explicit Ghost(EVA::GameObject* gameObject, Game* game);

	void Update(float deltaTime) override;

	void Reset();

private:

	static glm::ivec2 DirectionToVector(Direction direction);
	static Direction VectorToDirection(glm::ivec2 direction);

	std::vector<Direction> FindPossibleDirections() const;

	Direction ChooseDirection(const std::vector<Ghost::Direction>& directions) const;
	Direction DirectionToTarget(const std::vector<Ghost::Direction>& directions, glm::ivec2 target) const;
	
	
};