#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "BoundingBox.hpp"

namespace EVA
{

	class UiElement
	{
	protected:

		UiElement* m_Parent;
		std::vector<UiElement *> m_Children;

		glm::vec2 m_Position;
		BoundingBox m_BoundingBox;

	public:

		const glm::vec2& position = m_Position;
		const BoundingBox& boundingBox = m_BoundingBox;
		
		UiElement();

		void SetParent(UiElement* newParent);

		UiElement* AddChild(UiElement * newChild);

		virtual void OnChildrenUpdated();


		int GetIndexOfChild(UiElement* child) const;
		UiElement * GetChild(unsigned int index) const;

		virtual void SetPosition(glm::vec2 newPosition);

		virtual void Update() {}

		virtual void Render() const {}

		virtual void UpdateBoundingBox();
	};
}
