#include "EVA.hpp"
#include "EVA/SceneManager.hpp"

#include "Game.hpp"
#include "MainMenu.hpp"

int main()
{
    EVA::Application::Init("Pacman");
    EVA::SceneManager::CreateScene<MainMenu>();
    EVA::Application::Run();

    return 0;
}
