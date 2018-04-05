#pragma once

#include "EVA.hpp"

class Pacman : public EVA::Component
{
	std::shared_ptr<EVA::Mesh> m_Mesh;
	std::shared_ptr<EVA::Material> m_Material;

public:
	explicit Pacman(EVA::GameObject* gameObject);
};