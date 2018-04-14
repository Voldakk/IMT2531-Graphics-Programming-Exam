#pragma once

#include "EVA.hpp"
#include "EVA/Graphics.hpp"

class Ghost : public EVA::Component
{
	std::shared_ptr<EVA::Model> m_Model;
	std::shared_ptr<EVA::Material> m_BodyMaterial;
	std::shared_ptr<EVA::Material> m_EyeMaterial;

public:
	explicit Ghost(EVA::GameObject* gameObject, glm::vec3 color);
};