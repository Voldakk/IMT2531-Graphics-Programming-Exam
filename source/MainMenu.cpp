#include "MainMenu.hpp"

#include "EVA/UI.hpp"

MainMenu::MainMenu()
{
	const auto labelTitle = CreateUiElement<EVA::Label>("PACMAN", 0.005f);
	const auto labelStart = CreateUiElement<EVA::Label>("START", 0.002f);
	const auto labelExit = CreateUiElement<EVA::Label>("EXIT", 0.002f);

	auto layout = CreateUiElement<EVA::Layout>();
	layout->SetMargin(0.02f);
	layout->AddChild(labelTitle);
	layout->AddChild(labelStart);
	layout->AddChild(labelExit);
}