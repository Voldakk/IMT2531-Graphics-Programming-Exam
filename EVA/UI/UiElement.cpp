#include "UiElement.hpp"

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

	void UiElement::AddChild(UiElement* newChild)
	{
		if(newChild == nullptr)
			return;
		m_Children.push_back(newChild);
		newChild->m_Parent = this;

		OnChildredUpdated();
	}

	void UiElement::OnChildredUpdated()
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

	void UiElement::SetPosition(const glm::vec2 newPosition)
	{
		m_Position = newPosition;
		UpdateBoundingBox();
	}

	void UiElement::UpdateBoundingBox()
	{

	}
}
