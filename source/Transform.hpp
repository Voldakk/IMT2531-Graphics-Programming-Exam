#pragma once

#include "glm/glm/glm.hpp"

#include "Component.hpp"

class Transform : public Component
{
public:

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Transform();

	void Translate(glm::vec3 offset);
	void SetPosition(glm::vec3 newPosition);

	void Rotate(glm::vec3 offset);
	void SetRotation(glm::vec3 newRotation);

	void Scale(glm::vec3 offset);
	void SetScale(glm::vec3 newScale);

	void UpdateModelMatrix();

	glm::mat4 GetModelMatrix();

private:
	glm::mat4 model;
};