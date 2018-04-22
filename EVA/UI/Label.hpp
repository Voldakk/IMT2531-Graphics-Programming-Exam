#pragma once

#include "UiElement.hpp"

#include <string>

#include "Text.hpp"
#include "SpriteRenderer.hpp"

namespace EVA
{

	class Label : public UiElement
	{
		std::unique_ptr<Text> m_TextRenderer;
		std::unique_ptr<SpriteRenderer> m_Sr;

		std::string m_Text;
		glm::vec3 m_Color;		

		float m_TextScale;

	public:

		explicit Label(const std::string& text, float textScale = 0.001f);

		void UpdateBoundingBox() override;

		void Render() const override;
		
		void SetText(const std::string& newText);
		void SetScale(float newScale);

	};

}
