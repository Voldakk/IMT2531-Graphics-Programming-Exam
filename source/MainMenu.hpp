#pragma once

#include "EVA.hpp"
#include "EVA/UI.hpp"
#include "EVA/Input.hpp"
#include "EVA/SceneManager.hpp"

#include "Game.hpp"

class MainMenu : public EVA::Scene
{

public:

	MainMenu()
	{
		EVA::Input::SetCursorMode(EVA::Input::Normal);

		auto startButton = CreateUiElement<EVA::Button>("START", 0.002f);
		startButton->onClick = []
		{
			EVA::SceneManager::ChangeScene<Game>();
		};

		auto exitButton = CreateUiElement<EVA::Button>("EXIT", 0.002f);
		exitButton->onClick = []
		{
			EVA::Application::Exit();
		};

		auto layout = CreateUiElement<EVA::Layout>();
		layout->SetMargin(0.02f);

		layout->AddChild(CreateUiElement<EVA::Label>("PACMAN", 0.005f));
		layout->AddChild(startButton);
		layout->AddChild(exitButton);

	}
};