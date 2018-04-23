#pragma once

#include "UiElement.hpp"

namespace EVA
{

	class Layout : public UiElement
	{
		float m_Margin;

	public:
		explicit Layout();
		void OnChildrenUpdated() override;
		void UpdateBoundingBox() override;

		void SetMargin(float newMargin);
	};

}
