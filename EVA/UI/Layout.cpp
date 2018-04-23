#include "Layout.hpp"

EVA::Layout::Layout()
{
	m_Margin = 0.0f;
}

void EVA::Layout::OnChildrenUpdated()
{
	UpdateBoundingBox();
}

void EVA::Layout::UpdateBoundingBox()
{
	auto totalHeight = 0.0f;

	for (const auto& child : m_Children)
	{
		totalHeight += child->boundingBox.Height();
		totalHeight += m_Margin;
	}
	totalHeight -= m_Margin;

	auto height = -totalHeight / 2.0f;

	for (int i = m_Children.size() - 1; i >= 0; --i)
	{
		auto& child = m_Children[i];

		const auto offset = child->position.y - child->boundingBox.min.y;

		child->SetPosition(glm::vec2(position.x - child->boundingBox.Width() / 2, height + offset));

		height += child->boundingBox.Height();
		height += m_Margin;
	}

	m_BoundingBox.min = position;
	m_BoundingBox.max = position + glm::vec2(1.0f, totalHeight);
}

void EVA::Layout::SetMargin(const float newMargin)
{
	m_Margin = newMargin;
	UpdateBoundingBox();
}
