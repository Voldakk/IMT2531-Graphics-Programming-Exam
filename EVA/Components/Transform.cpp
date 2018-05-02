#include "Transform.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../GameObject.hpp"

namespace EVA
{

	Transform::Transform()
	{
		m_Parent = nullptr;
		UpdateModelMatrix();
	}

	void Transform::Translate(const glm::vec3 offset)
	{
		m_LocalPosition += offset;
		UpdateModelMatrix();
	}

	void Transform::Translate(const glm::vec2 offset)
	{
		m_LocalPosition += glm::vec3(offset, 0.0f);
		UpdateModelMatrix();
	}

	void Transform::SetPosition(const glm::vec3 newPosition)
	{
		m_LocalPosition = newPosition;
		UpdateModelMatrix();
	}

	void Transform::SetPosition(const glm::vec2 newPosition)
	{
		m_LocalPosition = glm::vec3(newPosition, 0.0f);
		UpdateModelMatrix();
	}

	void Transform::Rotate(const glm::quat offset)
	{
		m_LocalOrientation = offset * m_LocalOrientation;
		UpdateModelMatrix();
	}

	void Transform::Rotate(const glm::vec3 axis, const float angle)
	{
		m_LocalOrientation = glm::angleAxis(glm::radians(angle), axis) * m_LocalOrientation;
		UpdateModelMatrix();
	}

	void Transform::Rotate(const float angle)
	{
		m_LocalOrientation = glm::angleAxis(glm::radians(angle), YAXIS) * m_LocalOrientation;
		UpdateModelMatrix();
	}

	void Transform::SetOrientation(const glm::quat newOrientation)
	{
		m_LocalOrientation = newOrientation;
		UpdateModelMatrix();
	}

	void Transform::SetOrientation(const glm::vec3 axis, const float angle)
	{
		m_LocalOrientation = glm::angleAxis(glm::radians(angle), axis);
		UpdateModelMatrix();
	}

	void Transform::SetOrientation(const float angle)
	{
		m_LocalOrientation = glm::angleAxis(glm::radians(angle), YAXIS);
		UpdateModelMatrix();
	}

	void Transform::Scale(const glm::vec3 amount)
	{
		m_LocalScale += amount;
		UpdateModelMatrix();
	}

	void Transform::Scale(const float amount)
	{
		m_LocalScale += glm::vec3(amount);
		UpdateModelMatrix();
	}

	void Transform::SetScale(const glm::vec3 newScale)
	{
		m_LocalScale = newScale;
		UpdateModelMatrix();
	}

	void Transform::SetScale(const float newScale)
	{
		m_LocalScale = glm::vec3(newScale);
		UpdateModelMatrix();
	}

	void Transform::UpdateModelMatrix()
	{
		// Updates the world position, orientation and scale
		m_Position = m_Parent == nullptr ? m_LocalPosition : glm::vec3(m_Parent->modelMatrix * glm::vec4(m_LocalPosition, 1.0f));
		m_Orientation = m_Parent == nullptr ? m_LocalOrientation : m_LocalOrientation * m_Parent->m_Orientation;
		m_Scale = m_Parent == nullptr ? m_LocalScale : m_Parent->m_Scale * m_LocalScale;
		
		// Parent
		m_ModelMatrix = m_Parent == nullptr ? glm::mat4() : m_Parent->modelMatrix;

		// Position
		m_ModelMatrix = glm::translate(m_ModelMatrix, m_LocalPosition);

		// Orientation
		auto o = m_LocalOrientation;
		o.y *= -1;
		o.z *= -1;
		m_ModelMatrix = m_ModelMatrix * glm::toMat4(o);

		// Scale
		m_ModelMatrix = glm::scale(m_ModelMatrix, m_LocalScale);

		// Directions
		m_Forward = glm::normalize(ZAXIS * orientation);
		m_Right = glm::normalize(-XAXIS * orientation);
		m_Up = glm::normalize(YAXIS * orientation);

		// Update the children
		for (auto child : m_Children)
		{
			child->UpdateModelMatrix();
		}
	}

	void Transform::SetParent(GameObject *newParent)
	{
		if (newParent != nullptr)
			SetParent(newParent->transform.get());
	}

	void Transform::SetParent(Transform* newParent)
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
		m_Parent = newParent;

		// Add the transform to the new parents list of children
		if (m_Parent != nullptr)
			m_Parent->m_Children.push_back(this);

		// Update the model matrix
		UpdateModelMatrix();
	}

	int Transform::GetChildIndex(Transform *child) const
	{
		for (unsigned int i = 0; i < m_Children.size(); i++)
		{
			if (child == m_Children[i])
				return i;
		}
		return -1;
	}

	int Transform::GetChildIndex(GameObject *child) const
	{
		if (child != nullptr)
			return GetChildIndex(child->transform.get());
		return -1;
	}

	glm::vec3 Transform::LocalToWorld(const glm::vec3 localPosition) const
	{
		auto o = orientation;

		return localPosition * o;
	}
}