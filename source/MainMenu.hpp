#pragma once

#include "EVA-Engine/Menu.hpp"
#include "EVA-Engine/Sprite.hpp"

// The game's main menu
class MainMenu : public Menu
{
public:

	/// <summary>Constructor. Creates the buttons for the menu and the logo</summary>
	MainMenu();

	/// <summary>Renders the logo to the screen</summary>
	void Render() override;

private:
	// Pacman logo
	Sprite logo;

	/// <summary>Called when a button in the menu is pressed</summary>
	/// <param name="i">The index of the pressed button</param>   
	void OnButtonPressed(int i) override;
};