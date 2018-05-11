#include "EVA.hpp"
#include "EVA/SceneManager.hpp"
#include "../EVA/Editor/SceneEditor.hpp"

#include "Components/Terrain.hpp"

/**
 * \brief The entry point of the program
 * \return An exit code indicating how the program exited
 */
int main()
{
    EVA::Application::Init("EVA-Engine");

	EVA::SceneManager::CreateScene<EVA::SceneEditor>();
	//EVA::SceneManager::CreateScene<EVA::Scene>("./assets/scenes/cubes.scene");

    EVA::Application::Run();

    return EXIT_SUCCESS;
}
