#pragma once

#include "EVA.hpp"
#include "TileMap.hpp"

class Pacman : public EVA::Component
{
	enum Direction { Up, Down, Right, Left };

	std::shared_ptr<EVA::Mesh> m_Mesh;
	std::shared_ptr<EVA::Material> m_Material;
	std::shared_ptr<TileMap> m_TileMap;

	glm::vec3 m_CurrentDirection;
	Direction m_InputDirection;

	float m_MovementSpeed;

public:

	explicit Pacman(EVA::GameObject* gameObject, std::shared_ptr<TileMap> tileMap);
	void Update(float deltaTime) override;
};
