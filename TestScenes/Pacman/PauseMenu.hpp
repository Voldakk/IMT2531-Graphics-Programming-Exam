#pragma once

#include "EVA.hpp"
#include "EVA/UI.hpp"
#include "EVA/Input.hpp"
#include "EVA/SceneManager.hpp"

#include "MainMenu.hpp"
#include "Game.hpp"

class PauseMenu : public EVA::Scene
{

	Game* m_Game;

public:

	explicit PauseMenu(Game * game) : m_Game(game)
	{
		EVA::Input::SetCursorMode(EVA::Input::Normal);

		auto startButton = CreateUiElement<EVA::Button>("RESUME", 0.002f);
		startButton->onClick = [&]
		{
			m_Game->Unpause();
			EVA::SceneManager::UnloadScene(self);
		};

		auto exitButton = CreateUiElement<EVA::Button>("MAIN MENU", 0.002f);
		exitButton->onClick = []
		{
			EVA::SceneManager::ChangeScene<MainMenu>();
		};

		auto layout = CreateUiElement<EVA::Layout>();
		layout->SetMargin(0.05f);

		layout->AddChild(CreateUiElement<EVA::Label>("PAUSED", 0.005f));

		layout->AddChild(CreateUiElement<EVA::Padding>(glm::vec2(0.2f)));

		layout->AddChild(startButton);
		layout->AddChild(exitButton);
	}

	void Update(const float deltaTime) override
	{
		Scene::Update(deltaTime);

		if (EVA::Input::KeyDown(EVA::Input::Escape))
		{
			m_Game->Unpause();
			EVA::SceneManager::UnloadScene(this);
		}
	}
};
