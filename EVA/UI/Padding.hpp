#pragma once

#include "glm/glm.hpp"

#include "UiElement.hpp"

namespace EVA
{

	class Padding : public UiElement
	{
		glm::vec2 m_Size;

	public:
		explicit Padding(const glm::vec2 size) : UiElement()
		{
			m_Size = size;
			UpdateBoundingBox();
		}

		void UpdateBoundingBox() override
		{
			const auto halfSize = m_Size / 2.0f;
			m_BoundingBox = BoundingBox(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y);
		}
	};

}
