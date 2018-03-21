#pragma once

#include "glm/glm/glm.hpp"

#include "Component.hpp"
#include <vector>
#include <memory>

class GameObject;

class Transform : public Component
{
public:

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	std::shared_ptr<Transform> parent;
	std::vector<Transform*> children;

	explicit Transform(GameObject * gameObject);

	void Translate(glm::vec3 offset);
	void SetPosition(glm::vec3 newPosition);

	void Rotate(glm::vec3 offset);
	void SetRotation(glm::vec3 newRotation);

	void Scale(glm::vec3 offset);
	void SetScale(glm::vec3 newScale);

	void UpdateModelMatrix();

	glm::mat4 GetModelMatrix() const;

	void SetParent(GameObject * newParent);
	void SetParent(std::shared_ptr<Transform>& newParent);

	int GetChildIndex(Transform * child) const;
	int GetChildIndex(GameObject * child) const;

private:
	glm::mat4 model;
};
