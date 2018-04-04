#pragma once

#include "glm/glm.hpp"

namespace EVA
{

	enum LightType
	{
		Directional, Point
	};

	class Light
	{

		// Directional light
		glm::vec4 m_Direction;
		glm::vec2 m_Rotation = { 30.0f, 0.0f };

	public:

		// Common
		LightType type = LightType::Directional;
		glm::vec3 color = glm::vec3(1.0f);
		float ambientCoefficient = 0.005f;

		// Point light
		glm::vec3 position;
		float attenuation = 0.2f;

		void SetRotation(glm::vec2 rotation);
		glm::vec4 GetDirection() const { return m_Direction; }
	};

}