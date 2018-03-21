#include "Camera.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Input.hpp"

Camera::Camera()
{
	position = glm::vec3(0.0f);

	pitch = 0.0f;
	yaw = 90.0f;

	fov = 90.0f;
	near = 0.1f;
	far = 1000.0f;

	UpdateDirections();
}

void Camera::Update(const float deltaTime)
{
	// Movement

	if (Input::Key(GLFW_KEY_W))
		position += front * movementSpeed * deltaTime;
	if (Input::Key(GLFW_KEY_S))
		position -= front * movementSpeed * deltaTime;

	if (Input::Key(GLFW_KEY_D))
		position += right * movementSpeed * deltaTime;
	if (Input::Key(GLFW_KEY_A))
		position -= right * movementSpeed * deltaTime;

	if (Input::Key(GLFW_KEY_SPACE))
		position += up * movementSpeed * deltaTime;
	if (Input::Key(GLFW_KEY_LEFT_SHIFT))
		position -= up * movementSpeed * deltaTime;

	// Look
	const auto mouseMovement = Input::MouseMovement();

	yaw += mouseMovement.x * mouseSensitivity;
	pitch -= mouseMovement.y * mouseSensitivity;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	UpdateDirections();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return viewMatrix;
}

void Camera::UpdateDirections()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f })); 
	up = glm::normalize(glm::cross(right, front));

	viewMatrix = glm::lookAt(position, position + front, up);
}
