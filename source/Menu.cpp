#include "Menu.hpp"

#include "Input.hpp"
#include "Sprite.hpp"

// Destructor
Menu::~Menu()
{
	for (auto& button : buttons)
	{
		delete(button);
	}
	for (auto& text : texts)
	{
		delete(text);
	}
}

// Called every frame
void Menu::Update(float deltaTime)
{
	// Press selected button
	if (Input::KeyDown(GLFW_KEY_ENTER) || Input::KeyDown(GLFW_KEY_SPACE))
	{
		OnButtonPressed(selected);
	}

	// Navigate buttons
	if (Input::KeyDown(GLFW_KEY_UP) || Input::KeyDown(GLFW_KEY_W))
	{
		if (selected != 0)
		{
			buttons[selected]->SetAnimation("normal");
			buttons[--selected]->SetAnimation("selected");
		}
	}
	else if (Input::KeyDown(GLFW_KEY_DOWN) || Input::KeyDown(GLFW_KEY_S))
	{
		if (selected != buttons.size() - 1)
		{
			buttons[selected]->SetAnimation("normal");
			buttons[++selected]->SetAnimation("selected");
		}
	}
}

// Called every frame after Update
void Menu::Render()
{
	for (auto& button : buttons)
	{
		button->Render();
	}

	for (auto& text : texts)
	{
		text->Render();
	}
}

int Menu::CreateButton(const std::string& text, const glm::vec4 tint)
{
	// Button sprite
	Sprite * s = new Sprite();
	s->LoadSpriteSheet(spriteSheet, glm::vec2(2, 1));
	s->AddAnimation("normal", glm::uvec2(0, 0));
	s->AddAnimation("selected", glm::uvec2(1, 0));
	s->SetAnimation("normal");
	s->scale = buttonScale;
	s->position = offset + glm::vec2(0, buttons.size()*spacing);
	buttons.push_back(s);

	//Text
	if (!text.empty())
	{
		const auto t = new Text(text, s->position, true, s->scale.y * 0.8f, tint);
		texts.push_back(t);
	}

	return buttons.size() - 1;
}

void Menu::SetSelected(const size_t i)
{
	if (i >= buttons.size() || buttons[i] == nullptr ||
		selected >= buttons.size() || buttons[selected] == nullptr)
		return;

	buttons[selected]->SetAnimation("normal");
	buttons[i]->SetAnimation("selected");

	selected = i;
}