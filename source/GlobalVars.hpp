#pragma once

#include <string>

#include "glm/glm/glm.hpp"

const std::string APPLICATION_NAME = "EVA-Engine";

const int MAX_FPS = 200;										// The max fps the game can run at
const int WINDOW_WIDTH = 1024;									// The window's width in pixels
const int WINDOW_HEIGHT = 1024;									// The window's height in pixels

const glm::vec4 MENU_TEXT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };	// The menu's default text color
const glm::vec4 BUTTON_TEXT_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f };	// The menu's default text color
const glm::vec4 BACKGROUND_COLOR = { 0.5f, 0.5f, 0.5f, 1.0f };	// The GL clear color

// Starting scene
class MainMenu;
typedef MainMenu StartScene;