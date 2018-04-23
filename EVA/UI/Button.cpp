#include "Button.hpp"

#include <iostream>

#include "../Input.hpp"

EVA::Button::Button(const std::string& text, const float textScale)
	: Label(text, textScale)
{
	m_State = Normal;
	m_Sr = std::make_unique<SpriteRenderer>();

	onClick = [text]
	{
		std::cout << "Button::onClick - " << text << " was clicked \n";
	};
}

void EVA::Button::Update()
{
	const auto mousePos = Input::ScreenToWorldPos(Input::MousePosition());
	const auto mouseOver = m_BoundingBox.ContainsPoint(mousePos);

	if (m_State == Normal && mouseOver)
		m_State = Highlighted;
	else if (m_State == Highlighted && !mouseOver)
		m_State = Normal;

	if (mouseOver && Input::MouseButtonDown(0))
		onClick();
}

void EVA::Button::Render() const
{
	if(m_State == Highlighted)
	{
		m_Sr->Render(m_BoundingBox);
	}

	Label::Render();
}
