#include "EVA-Engine/SceneManager.hpp"
#include "EVA-Engine/Application.hpp"

#include "Game.hpp"

int main()
{
	Application::Init("Pacman");
	SceneManager::CreateScene<Game>();
	Application::Run();

	return 0;
}