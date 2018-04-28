#pragma once

#include "EVA.hpp"

#include "TileMap.hpp"

class Pacman : public EVA::Component, public EVA::IUpdateComponent
{

public:

	enum Direction { Up, Right, Down, Left, Last };
	enum InputMode { Global, Local };

private:

	std::shared_ptr<EVA::Model> m_Model;
	std::shared_ptr<EVA::Material> m_Material;
	TileMap* m_TileMap;

	glm::ivec2 m_CurrentTile;
	glm::ivec2 m_TargetTile;

	Direction m_InputDirection;
	Direction m_CurrentOrientation;
	glm::ivec2 m_CurrentDirection;

	float m_MovementSpeed;

public:

	glm::ivec2& currentTile = m_CurrentTile;
	glm::ivec2& currentDirection = m_CurrentDirection;

	InputMode inputMode = Local;

	explicit Pacman(EVA::GameObject* gameObject, TileMap* tileMap);
	void Update(float deltaTime) override;
	void Reset();
};