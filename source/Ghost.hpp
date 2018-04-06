#pragma once

#include "EVA.hpp"

class Ghost : public EVA::Component
{
	std::vector<std::shared_ptr<EVA::Mesh>> m_Mesh;
	std::shared_ptr<EVA::Material> m_BodyMaterial;
	std::shared_ptr<EVA::Material> m_EyeMaterial;

public:
	explicit Ghost(EVA::GameObject* gameObject, glm::vec3 color);
};