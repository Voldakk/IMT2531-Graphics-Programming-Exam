#pragma once
#include <vector>

#include "Text.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "GlobalVars.hpp"

// A generic base calss for menus
class Menu : public Scene
{
public:

	/// <summary>Destructor. Deletes all the buttons in the menu</summary>
	~Menu();

	/// <summary>Called every frame. Allows the usser to select and press buttons</summary>
	/// <param name="deltaTime">The time in seconds between frames</param>
	void Update(float deltaTime) override;

	/// <summary>Render all the buttons in the menu to the screen</summary>
	void Render() override;

protected:
	std::string spriteSheet;

	glm::vec2 offset;
	glm::vec2 buttonScale;
	float spacing;

	size_t selected = 0;
	std::vector<Sprite*> buttons;
	std::vector<Text*> texts;


	/// <summary>Creates a new button</summary>
	/// <param name="text">The text on the button</param>  
	/// <param name="tint">The text color</param>  
	/// <returns>The index of the button in the menu</returns>  
	int CreateButton(std::string text = "", glm::vec4 tint = BUTTON_TEXT_COLOR);

	/// <summary>Sets the currently selected button</summary>
	/// <param name="i">The index of the newly selected button</param>  
	void SetSelected(size_t i);

	/// <summary>Virtual function called when a button in the menu is pressed</summary>
	/// <param name="i">The index of the pressed button</param>   
	virtual void OnButtonPressed(int i) = 0;
};