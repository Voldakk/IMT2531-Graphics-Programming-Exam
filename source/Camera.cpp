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
	auto position = gameObject->transform->position;

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

	gameObject->transform->SetPosition(position);

	// Look
	auto rotation = gameObject->transform->rotation;

	const auto mouseMovement = Input::MouseMovement();
	rotation += glm::vec3( -mouseMovement.y , mouseMovement.x, 0) * mouseSensitivity;

	if (rotation.x < -89.0f)
		rotation.x = -89.0f;
	else if (rotation.x > 89.0f)
		rotation.x = 89.0f;

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

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f })); 
	up = glm::normalize(glm::cross(right, front));

	viewMatrix = glm::lookAt(position, position + front, up);
}