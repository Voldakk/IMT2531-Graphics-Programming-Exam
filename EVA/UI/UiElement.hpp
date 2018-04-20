#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "BoundingBox.hpp"

namespace EVA
{

	class UiElement
	{
	protected:

		UiElement* m_Parent;
		std::vector<UiElement*> m_Children;

		glm::vec2 m_Position;
		BoundingBox m_BoundingBox;

	public:

		/*const*/ glm::vec2& position = m_Position;
		/*const*/ BoundingBox& boundingBox = m_BoundingBox;
		
		UiElement();

		void SetParent(UiElement* newParent);
		void SetParent(const std::shared_ptr<UiElement>& newParent);

		int GetIndexOfChild(UiElement *child) const;
		UiElement * GetChild(unsigned int index) const;

		virtual void Render(){}
	};

}