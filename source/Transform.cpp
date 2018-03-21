#include "Transform.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "GameObject.hpp"

Transform::Transform(GameObject * gameObject) : Component(gameObject)
{
	parent = nullptr;
	UpdateModelMatrix();
}

void Transform::Translate(const glm::vec3 offset)
{
	position += offset;
	UpdateModelMatrix();
}

void Transform::SetPosition(const glm::vec3 newPosition)
{
	position = newPosition;
	UpdateModelMatrix();
}

void Transform::Rotate(const glm::vec3 offset)
{
	rotation += offset;
	UpdateModelMatrix();
}

void Transform::SetRotation(const glm::vec3 newRotation)
{
	rotation = newRotation;
	UpdateModelMatrix();
}

void Transform::Scale(const glm::vec3 offset)
{
	scale += offset;
	UpdateModelMatrix();
}

void Transform::SetScale(const glm::vec3 newScale)
{
	scale = newScale;
	UpdateModelMatrix();
}

void Transform::UpdateModelMatrix()
{
	model = parent == nullptr ?  glm::mat4() : parent->GetModelMatrix();
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));

	for (auto child : children)
	{
		child->UpdateModelMatrix();
	}
}

glm::mat4 Transform::GetModelMatrix() const
{
	return model;
}

void Transform::SetParent(GameObject* newParent)
{
	if (newParent != nullptr)
		SetParent(newParent->transform);
}

void Transform::SetParent(std::shared_ptr<Transform>& newParent)
{
	// If the transform has a parent
	if (parent != nullptr)
	{
		// Remove the transform from the parents list of children
		const auto index = parent->GetChildIndex(this);
		if (index != -1)
			parent->children.erase(parent->children.begin() + index);
	}

	// Set new parent
	parent = newParent;

	// Add the transform to the new parents list of children
	if (parent != nullptr)
		parent->children.push_back(this);

	// Update the model matrix
	UpdateModelMatrix();
}

int Transform::GetChildIndex(Transform* child) const
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (child == children[i])
			return i;
	}
	return -1;
}

int Transform::GetChildIndex(GameObject* child) const
{
	if (gameObject != nullptr)
		return GetChildIndex(gameObject->transform.get());
	return -1;
}