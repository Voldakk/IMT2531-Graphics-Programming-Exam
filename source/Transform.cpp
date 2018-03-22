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
	localPosition += offset;
	UpdateModelMatrix();
}

void Transform::SetPosition(const glm::vec3 newPosition)
{
	localPosition = newPosition;
	UpdateModelMatrix();
}

void Transform::Rotate(const glm::vec3 offset)
{
	localRotation += offset;
	UpdateModelMatrix();
}

void Transform::SetRotation(const glm::vec3 newRotation)
{
	localRotation = newRotation;
	UpdateModelMatrix();
}

void Transform::Scale(const glm::vec3 offset)
{
	localScale += offset;
	UpdateModelMatrix();
}

void Transform::SetScale(const glm::vec3 newScale)
{
	localScale = newScale;
	UpdateModelMatrix();
}

void Transform::UpdateModelMatrix()
{
	rotation = parent == nullptr ? localRotation : parent->rotation + localRotation;
	scale = parent == nullptr ? localScale : parent->scale * localScale;
	position = parent == nullptr ? localPosition : glm::vec3(parent->GetModelMatrix() * glm::vec4(localPosition, 1.0f));

	model = parent == nullptr ?  glm::mat4() : parent->GetModelMatrix();
	model = glm::translate(model, localPosition);
	
	model = glm::rotate(model, localRotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, localRotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, localRotation.z, glm::vec3(0, 0, 1));

	model = glm::scale(model, localScale);

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