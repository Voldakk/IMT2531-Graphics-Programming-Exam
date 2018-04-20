#pragma once

#include "UiElement.hpp"

#include <string>

#include "Text.hpp"

namespace EVA
{

	class Label : public UiElement
	{
		std::unique_ptr<Text> m_TextRenderer;

		std::string m_Text;
		glm::vec3 m_Color;		

	public:

		explicit Label(std::string text);

		void Render() override;
		
		void SetText(std::string newText);
	};

}
