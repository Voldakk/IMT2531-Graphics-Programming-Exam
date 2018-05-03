#include "EVA.hpp"
#include "EVA/SceneManager.hpp"

#include "../TestScenes/Pacman/MainMenu.hpp"
#include "../TestScenes/UiTest.hpp"
#include "../TestScenes/Transforms.hpp"

/**
 * \brief The entry point of the program
 * \return An exit code indicating how the program exited
 */
int main()
{
    EVA::Application::Init("EVA-Engine");

	//EVA::SceneManager::CreateScene<MainMenu>();               // The Pacman game

	//EVA::SceneManager::CreateScene<EVA_TEST::Transforms>(); // Shows transform parents and multiple directional lights
	//EVA::SceneManager::CreateScene<EVA_TEST::UiTest>();     // Shows buttons, and UI anchor and pivots

	EVA::SceneManager::CreateScene("./assets/scenes/cubes.scene");

    EVA::Application::Run();

    return EXIT_SUCCESS;
}
