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

	glm::ivec2 m_CurrentTile;
	glm::ivec2 m_TargetTile;

	Direction m_InputDirection = Up;
	Direction m_CurrentOrientation = Up;
	glm::ivec2 m_CurrentDirection;

	float m_MovementSpeed = 0.0f;

public:
	TileMap* tileMap = nullptr;

	glm::ivec2& currentTile = m_CurrentTile;
	glm::ivec2& currentDirection = m_CurrentDirection;

	InputMode inputMode = Local;

	void Start() override;
	void Update(float deltaTime) override;
	void Reset();
};