#include "Label.hpp"

#include <utility>

EVA::Label::Label(std::string text) 
	: m_Text(std::move(text)), m_Color(glm::vec3(1.0f))
{
	m_TextRenderer = std::make_unique<Text>();
}

void EVA::Label::Render()
{
	m_TextRenderer->Render(m_Text, m_Position.x, m_Position.y, 1.0f, m_Color);
}

void EVA::Label::SetText(const std::string newText)
{
	m_Text = newText;
}
