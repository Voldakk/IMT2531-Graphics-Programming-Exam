#include "EVA.hpp"
#include "EVA/SceneManager.hpp"

#include "../TestScenes/UiTest.hpp"
#include "../EVA/Editor/SceneEditor.hpp"

#include <experimental/filesystem>

/**
 * \brief The entry point of the program
 * \return An exit code indicating how the program exited
 */
int main()
{
    std::cout << std::experimental::filesystem::current_path() << std::endl;

    EVA::Application::Init("EVA-Engine");

	EVA::SceneManager::CreateScene<EVA::SceneEditor>("./assets/scenes/cubes.scene");

    EVA::Application::Run();

    return EXIT_SUCCESS;
}
