#include "Menu.hpp"

#include "Input.hpp"
#include "Sprite.hpp"

// Destructor
Menu::~Menu()
{
	for (size_t i = 0; i < buttons.size(); i++)
	{
		delete(buttons[i]);
	}
	for (size_t i = 0; i < texts.size(); i++)
	{
		delete(texts[i]);
	}
}

// Called every frame
void Menu::Update(float deltaTime)
{
	// Press selected button
	if (Input::GetKeyDown(GLFW_KEY_ENTER) || Input::GetKeyDown(GLFW_KEY_SPACE))
	{
		OnButtonPressed(selected);
	}

	// Navigate buttons
	if (Input::GetKeyDown(GLFW_KEY_UP) || Input::GetKeyDown(GLFW_KEY_W))
	{
		if (selected != 0)
		{
			buttons[selected]->SetAnimation("normal");
			buttons[--selected]->SetAnimation("selected");
		}
	}
	else if (Input::GetKeyDown(GLFW_KEY_DOWN) || Input::GetKeyDown(GLFW_KEY_S))
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
	for (size_t i = 0; i < buttons.size(); i++)
	{
		buttons[i]->Render();
	}

	for (size_t i = 0; i < texts.size(); i++)
	{
		texts[i]->Render();
	}
}

int Menu::CreateButton(std::string text, glm::vec4 tint)
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
	if (text != "")
	{
		Text * t = new Text(text, s->position, true, s->scale.y * 0.8f, tint);
		texts.push_back(t);
	}

	return buttons.size() - 1;
}

void Menu::SetSelected(size_t i)
{
	if (i < 0 || i >= buttons.size() || buttons[i] == nullptr ||
		selected < 0 || selected >= buttons.size() || buttons[selected] == nullptr)
		return;

	buttons[selected]->SetAnimation("normal");
	buttons[i]->SetAnimation("selected");

	selected = i;
}
