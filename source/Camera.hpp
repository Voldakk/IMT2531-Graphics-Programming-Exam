#pragma once

#include "glm/glm/glm.hpp"

#include "Component.hpp"

class GameObject;

class Camera : public Component
{
public:
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	float mouseSensitivity = 0.1f;
	float movementSpeed = 5.0f;

	float fov;
	float near;
	float far;

	explicit Camera(GameObject * gameObject);

	void Update(float deltaTime) override;

	glm::mat4 GetViewMatrix() const;

private:
	void UpdateDirections();

	glm::mat4 viewMatrix;
};

