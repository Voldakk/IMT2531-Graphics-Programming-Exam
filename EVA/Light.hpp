#pragma once

#include "glm/glm/glm.hpp"

struct Light
{
	glm::vec3 Position = glm::vec3(100.0f, 100.0f, 100.0f);
	glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);

	float Attenuation = 0.0f;

	float AmbientCoefficient = 0.005f;
};