#include "EVA.hpp"
#include "EVA/SceneManager.hpp"

#include "Game.hpp"

int main()
{
    EVA::Application::Init("Pacman");
    EVA::SceneManager::CreateScene<Game>();
    EVA::Application::Run();

    return 0;
}