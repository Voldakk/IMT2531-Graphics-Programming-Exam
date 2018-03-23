#include "Camera.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "Component.hpp"
#include "GameObject.hpp"

Camera::Camera(GameObject * gameObject) : Component(gameObject)
{
	fov = 90.0f;
	near = 0.1f;
	far = 1000.0f;

	UpdateDirections();
}

void Camera::Update(const float deltaTime)
{
	// Movement
	auto position = gameObject->transform->localPosition;

	if (Input::Key(GLFW_KEY_W)) // Front
		position += front * movementSpeed * deltaTime;
	if (Input::Key(GLFW_KEY_S))	// Back
		position -= front * movementSpeed * deltaTime;

	if (Input::Key(GLFW_KEY_D))	// Right
		position += right * movementSpeed * deltaTime;
	if (Input::Key(GLFW_KEY_A))	// Left
		position -= right * movementSpeed * deltaTime;

	if (Input::Key(GLFW_KEY_SPACE)) // Up
		position += up * movementSpeed * deltaTime;
	if (Input::Key(GLFW_KEY_LEFT_SHIFT)) // Down
		position -= up * movementSpeed * deltaTime;

	gameObject->transform->SetPosition(position);

	// Look
	auto rotation = gameObject->transform->localRotation;

	const auto mouseMovement = Input::MouseMovement();
	rotation += glm::vec3( -mouseMovement.y , mouseMovement.x, 0) * mouseSensitivity * deltaTime;

	if (rotation.x < glm::radians(-89.0f))
		rotation.x = glm::radians(-89.0f);
	else if (rotation.x > glm::radians(89.0f))
		rotation.x = glm::radians(89.0f);

	gameObject->transform->SetRotation(rotation);

	// Update view
	UpdateDirections();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return viewMatrix;
}

void Camera::UpdateDirections()
{
	const auto position = gameObject->transform->position;

	const auto pitch = gameObject->transform->rotation.x;
	const auto yaw = gameObject->transform->rotation.y;

	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f })); 
	up = glm::normalize(glm::cross(right, front));

	viewMatrix = glm::lookAt(position, position + front, up);
}