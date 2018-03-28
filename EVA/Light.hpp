#pragma once

#include "glm/glm/glm.hpp"

namespace EVA
{

	struct Light
	{
		enum LightType
		{
			Directional, Point
		};
		LightType Type;

		// Common
		glm::vec3 Color = glm::vec3(1.0f);
		float AmbientCoefficient = 0.005f;

		// Directional
		glm::vec2 Rotation;

		// Point
		glm::vec3 Position;
		float Attenuation = 0.2f;
	};

}