#include "EVA.hpp"
#include "EVA/SceneManager.hpp"
#include "../EVA/Editor/SceneEditor.hpp"

/**
 * \brief The entry point of the program
 * \return An exit code indicating how the program exited
 */
int main()
{

	if constexpr (false)
	{
		EVA::Application::Init("IMT2531 - Graphics Programming Take-Home Exam");
		EVA::SceneManager::CreateScene<EVA::Scene>("./assets/scenes/main.scene");
	}
	else
	{
		EVA::Application::Init("EVA-Engine");
		EVA::SceneManager::CreateScene<EVA::SceneEditor>("./assets/scenes/main.scene");
	}
    
	EVA::Application::Run();

    return EXIT_SUCCESS;
}
