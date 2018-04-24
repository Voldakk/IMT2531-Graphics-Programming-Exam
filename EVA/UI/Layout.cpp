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

		child->SetPivot(0.0f, -1.0f);
		child->SetPosition(glm::vec2(position.x, height));

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
