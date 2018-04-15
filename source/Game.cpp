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

	// Tilemap
	auto tileMapGo = CreateGameObject();
	tileMap = tileMapGo->AddComponent<TileMap>();
	tileMap->ReadFile("./assets/levels/level1.txt");

	// Camera
	auto camera = CreateGameObject();
	EVA::Application::mainCamera = camera->AddComponent<EVA::Camera>();
	camera->transform->SetPosition({ 0.0f, 3.0f, -2.0f });

	// Pacman
	auto pacmanGo = CreateGameObject();
	pacman = pacmanGo->AddComponent<Pacman>(tileMap);

	// Ghosts
	auto ghostGo = CreateGameObject();
	auto ghost = ghostGo->AddComponent<Ghost>(this);
	ghost->Reset();
}

void Game::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
}