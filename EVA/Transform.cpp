#include "Transform.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

#include "GameObject.hpp"

namespace EVA
{

	Transform::Transform(GameObject *gameObject) : Component(gameObject)
	{
		m_Parent = nullptr;
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
		rotation = m_Parent == nullptr ? localRotation : m_Parent->rotation + localRotation;
		scale = m_Parent == nullptr ? localScale : m_Parent->scale * localScale;
		position = m_Parent == nullptr ? localPosition : glm::vec3(
				m_Parent->GetModelMatrix() * glm::vec4(localPosition, 1.0f));

		m_Model = m_Parent == nullptr ? glm::mat4() : m_Parent->GetModelMatrix();
		m_Model = glm::translate(m_Model, localPosition);

		m_Model = glm::rotate(m_Model, localRotation.y, glm::vec3(0, 1, 0));
		m_Model = glm::rotate(m_Model, localRotation.x, glm::vec3(1, 0, 0));
		m_Model = glm::rotate(m_Model, localRotation.z, glm::vec3(0, 0, 1));

		m_Model = glm::scale(m_Model, localScale);

		for (auto child : m_Children)
		{
			child->UpdateModelMatrix();
		}
	}

	void Transform::SetParent(GameObject *newParent)
	{
		if (newParent != nullptr)
			SetParent(newParent->GetTransform());
	}

	void Transform::SetParent(std::shared_ptr<Transform> newParent)
	{
		// If the transform has a parent
		if (m_Parent != nullptr)
		{
			// Remove the transform from the parents list of children
			const auto index = m_Parent->GetChildIndex(this);
			if (index != -1)
				m_Parent->m_Children.erase(m_Parent->m_Children.begin() + index);
		}

		// Set new parent
		m_Parent = std::move(newParent);

		// Add the transform to the new parents list of children
		if (m_Parent != nullptr)
			m_Parent->m_Children.push_back(this);

		// Update the model matrix
		UpdateModelMatrix();
	}

	unsigned int Transform::GetChildIndex(Transform *child) const
	{
		for (unsigned int i = 0; i < m_Children.size(); i++)
		{
			if (child == m_Children[i])
				return i;
		}
		return -1;
	}

	unsigned int Transform::GetChildIndex(GameObject *child) const
	{
		if (m_GameObject != nullptr)
			return GetChildIndex(m_GameObject->GetTransform().get());
		return -1;
	}

}