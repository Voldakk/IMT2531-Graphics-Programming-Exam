#include "MainMenu.hpp"

#include "EVA-Engine/SceneManager.hpp"
#include "EVA-Engine/Application.hpp"

#include "Game.hpp"

// Constructor
MainMenu::MainMenu()
{
	// Pacman logo
	logo.LoadSpriteSheet("./assets/Logo.png", glm::vec2(1, 1));
	logo.AddAnimation("normal", glm::uvec2(0, 0));
	logo.SetAnimation("normal");
	logo.scale = glm::vec2(0.8f, 0.2f);
	logo.position.y = 0.4f;
	

	//Buttons
	spriteSheet = "./assets/Button.png";
	buttonScale = glm::vec2(0.5f, 0.1f);
	spacing = -0.3f;

	// Level 1
	CreateButton("LEVEL 1");
	SetSelected(0);

	// Level 2
	CreateButton("LEVEL 2");

	// Exit
	CreateButton("EXIT");
}

// Called every frame after Update
void MainMenu::Render()
{
	Menu::Render();

	logo.Render();
}

void MainMenu::OnButtonPressed(const int i)
{
	switch (i)
	{
		// Start button
		case 0:
			SceneManager::ChangeScene<Game>();
			return;

		// Start button
		case 1:
			SceneManager::ChangeScene<MainMenu>();
			return;

		// Exit button
		case 2:
			Application::Exit();
			return;
		default:
			break;
	}
}