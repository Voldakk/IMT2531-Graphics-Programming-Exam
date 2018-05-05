#include "EVA.hpp"
#include "EVA/SceneManager.hpp"

#include "../TestScenes/UiTest.hpp"
#include "../EVA/SceneEditor.hpp"

/**
 * \brief The entry point of the program
 * \return An exit code indicating how the program exited
 */
int main()
{
    EVA::Application::Init("EVA-Engine");

	EVA::SceneManager::CreateScene<EVA::SceneEditor>("./assets/scenes/cubes.scene");

    EVA::Application::Run();

    return EXIT_SUCCESS;
}
