#pragma once

#include "glm/glm/glm.hpp"

struct Light
{
	glm::vec3 position = glm::vec3(100.0f, 100.0f, 100.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	float attenuation = 0.0f;

	float ambientCoefficient = 0.005f;
};