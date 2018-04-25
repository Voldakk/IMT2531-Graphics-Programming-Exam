#pragma once

#include "EVA.hpp"

#include "TileMap.hpp"

class Pacman : public EVA::Component, public EVA::IUpdateComponent
{
	enum Direction { Up, Down, Right, Left };
	
	std::shared_ptr<EVA::Model> m_Model;
	std::shared_ptr<EVA::Material> m_Material;
	TileMap* m_TileMap;

	glm::ivec2 m_CurrentTile;
	glm::ivec2 m_TargetTile;

	Direction m_InputDirection;
	glm::ivec2 m_CurrentDirection;

	float m_MovementSpeed;

public:

	glm::ivec2& currentTile = m_CurrentTile;
	glm::ivec2& currentDirection = m_CurrentDirection;

	explicit Pacman(EVA::GameObject* gameObject, TileMap* tileMap);
	void Update(float deltaTime) override;
	void Reset();
};