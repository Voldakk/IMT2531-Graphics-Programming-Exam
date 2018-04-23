#include "Label.hpp"

EVA::Label::Label(const std::string& text, const float textScale) 
: m_Color(glm::vec3(1.0f)), m_TextScale(textScale)
{
	m_TextRenderer = std::make_unique<Text>();

	SetText(text);
}

void EVA::Label::UpdateBoundingBox()
{
	const auto bb = m_TextRenderer->GetSize(m_Text, m_TextScale);
	m_BoundingBox.min = m_Position;
	m_BoundingBox.max = m_Position + bb.max;
}


void EVA::Label::Render() const
{
	UiElement::Render();

	m_TextRenderer->Render(m_Text, position.x , position.y, m_TextScale, m_Color);
}

void EVA::Label::SetText(const std::string& newText)
{
	m_Text = newText;

	UpdateBoundingBox();

	if(m_Parent != nullptr)
		m_Parent->UpdateBoundingBox();
}

void EVA::Label::SetScale(const float newScale)
{
	m_TextScale = newScale;
	UpdateBoundingBox();
}