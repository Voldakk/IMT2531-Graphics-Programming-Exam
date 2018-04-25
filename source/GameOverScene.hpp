#pragma once

#include "EVA.hpp"
#include "EVA/UI.hpp"
#include "EVA/Input.hpp"
#include "EVA/SceneManager.hpp"

#include "MainMenu.hpp"

class GameOverScene : public EVA::Scene
{
public:
	GameOverScene(bool won, unsigned int score)
	{
		EVA::Input::SetCursorMode(EVA::Input::Normal);

		auto startButton = CreateUiElement<EVA::Button>("MAIN MENU", 0.002f);
		startButton->onClick = []
		{
			EVA::SceneManager::ChangeScene<MainMenu>();
		};

		auto layout = CreateUiElement<EVA::Layout>();
		layout->SetMargin(0.02f);

		layout->AddChild(CreateUiElement<EVA::Label>("GAME OVER", 0.005f));

		layout->AddChild(CreateUiElement<EVA::Label>(won ? "YOU WON!" : "YOU LOST!", 0.002f));
		layout->AddChild(CreateUiElement<EVA::Label>("SCORE: " + std::to_string(score), 0.002f));

		layout->AddChild(startButton);
	}
};
