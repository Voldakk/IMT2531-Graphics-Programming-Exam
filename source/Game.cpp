#include "Game.hpp"

#include "EVA/Graphics.hpp"
#include "EVA/SceneManager.hpp"
#include "EVA/ResourceManagers.hpp"

#include "TileMap.hpp"
#include "Ghost.hpp"
#include "Pacman.hpp"

Game::Game()
{
	// Shaders
	EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");
	EVA::ShaderManager::CreateOrGetShader("standard_instanced", "standard_instanced.vert", "standard_instanced.frag");

	// Skybox
	skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

	// A directional light that fits the skybox
	auto directionalLight = CreateLight(EVA::LightType::Directional, true);
	directionalLight->SetRotation({ 30.0f, -80.0 });
	directionalLight->directionalShadowDistance = 25.0f;

	//auto pointLight = CreateLight(EVA::LightType::Point, true, 128);
	//pointLight->SetPosition({ 11.0f, 0.25f, 9.5f });
	//pointLight->color = { 10.0f, 0.0f, 0.0f };
	//pointLight->attenuation = 0.0f;

	// Tilemap
	auto tileMapGo = CreateGameObject();
	auto tileMap = tileMapGo->AddComponent<TileMap>();
	tileMap->ReadFile("./assets/levels/level1.txt");

	// Camera
	auto camera = CreateGameObject();
	EVA::Application::mainCamera = camera->AddComponent<EVA::Camera>();
	camera->transform->SetPosition({ 0.0f, 3.0f, -2.0f });

	// Pacman
	auto pacman = CreateGameObject();
	pacman->AddComponent<Pacman>(tileMap);

	// Ghosts
	glm::vec3 colors[] = { { 1.0f, 0.0f, 0.0f }, { 0.85f, 0.35f, 0.85f }, { 0.5f, 0.9f, 0.9f }, { 0.95f, 0.8f, 0.3f } };

	for (unsigned int i = 0; i < 4; ++i)
	{
		auto ghost = CreateGameObject();
		ghost->AddComponent<Ghost>(colors[i]);
		ghost->transform->SetPosition({ (i + 1) * 2.0f, 1.0f, 0.0f });
		ghost->transform->SetOrientation(EVA::YAXIS, i * 90.0f);
	}
}

void Game::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
}