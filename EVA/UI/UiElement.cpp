#include "UiElement.hpp"
#include "../Application.hpp"

namespace EVA
{

	UiElement::UiElement() : m_BoundingBox(0.0f, 0.0f, 0.0f, 0.0f)
	{
		m_Parent = nullptr;
	}

	void UiElement::SetParent(UiElement* newParent)
	{
		if (m_Parent != nullptr)
		{
			const auto index = m_Parent->GetIndexOfChild(this);
			if (index != -1)
				m_Parent->m_Children.erase(m_Parent->m_Children.begin() + index);
		}

		m_Parent = newParent;

		if (m_Parent != nullptr)
			m_Parent->AddChild(this);
	}

	UiElement* UiElement::AddChild(UiElement* newChild)
	{
		if(newChild == nullptr)
			return nullptr;
		m_Children.push_back(newChild);
		newChild->m_Parent = this;

		OnChildrenUpdated();

		return newChild;
	}

	void UiElement::OnChildrenUpdated()
	{

	}

	int UiElement::GetIndexOfChild(UiElement *child) const
	{
		for (unsigned int i = 0; i < m_Children.size(); i++)
		{
			if (child == m_Children[i])
				return i;
		}
		return -1;
	}

	UiElement* UiElement::GetChild(const unsigned int index) const
	{
		if (index < m_Children.size())
			return m_Children[index];

		return nullptr;
	}

	void UiElement::SetPosition(const glm::vec2 newPosition, const bool usePivot)
	{
		m_Position = newPosition;

		if (usePivot)
		{
			if (m_Pivot.x > 0.0f)
				m_Position.x += m_BoundingBox.min.x * m_Pivot.x;
			else
				m_Position.x -= m_BoundingBox.max.x * m_Pivot.x;

			if (m_Pivot.y > 0.0f)
				m_Position.y += m_BoundingBox.min.y * m_Pivot.y;
			else
				m_Position.y -= m_BoundingBox.max.y * m_Pivot.y;
		}
	}

	void UiElement::UpdateBoundingBox()
	{

	}

	UiElement * UiElement::SetAnchor(const glm::vec2 newAnchor)
	{
		m_Anchor = newAnchor;
		UpdatePosition();

		return this;
	}

	UiElement* UiElement::SetAnchor(const float newAnchorX, const float newAnchorY)
	{
		return SetAnchor({ newAnchorX, newAnchorY });
	}

	UiElement* UiElement::SetPivot(const glm::vec2 newPivot)
	{
		m_Pivot = newPivot;
		UpdatePosition();

		return this;
	}

	UiElement* UiElement::SetPivot(const float newPivotX, const float newPivotY)
	{
		return SetPivot({ newPivotX, newPivotY });
	}

	UiElement * UiElement::SetOffset(const glm::vec2 newOffset)
	{
		m_Offset = newOffset;
		UpdatePosition();

		return this;
	}

	UiElement* UiElement::SetOffset(const float newOffsetX, const float newOffsetY)
	{
		return SetOffset({ newOffsetX, newOffsetY });
	}

	void UiElement::UpdatePosition()
	{
		const auto bounds = Application::GetOrthographicBounds();

		auto newPos = m_Anchor * bounds;
		newPos += m_Offset;

		SetPosition(newPos);
	}
}
