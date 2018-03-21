#pragma once

#include "glm/glm/glm.hpp"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	float mouseSensitivity = 0.1f;
	float movementSpeed = 5.0f;

	float pitch;
	float yaw;

	float fov;
	float near;
	float far;

	Camera();

	void Update(float deltaTime);

	glm::mat4 GetViewMatrix() const;

private:
	void UpdateDirections();

	glm::mat4 viewMatrix;
};

