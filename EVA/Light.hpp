#pragma once

#include "glm/glm/glm.hpp"

namespace EVA
{

	enum LightType
	{
		Directional, Point
	};

	class Light
	{

	private:

		// Directional light
		glm::vec4 m_Direction;
		glm::vec2 m_Rotation = { 30.0f, 0.0f };

	public:

		// Common
		LightType Type = LightType::Directional;
		glm::vec3 Color = glm::vec3(1.0f);
		float AmbientCoefficient = 0.005f;

		// Point light
		glm::vec3 Position;
		float Attenuation = 0.2f;

		void SetRotation(glm::vec2 rotation);
		glm::vec4 GetDirection() const { return m_Direction; }
	};

}