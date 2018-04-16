#include "Game.hpp"

#include "EVA/Graphics.hpp"
#include "EVA/SceneManager.hpp"
#include "EVA/ResourceManagers.hpp"

#include "TileMap.hpp"
#include "GhostVariations.hpp"
#include "Pacman.hpp"

Game::Game()
{
	// Shaders
	EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");
	EVA::ShaderManager::CreateOrGetShader("standard_instanced", "standard_instanced.vert", "standard_instanced.frag");

	EVA::ShaderManager::CreateOrGetShader("standard_transparent", "standard.vert", "standard.frag");

	// Skybox
	skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

	// A directional light that fits the skybox
	auto directionalLight = CreateLight(EVA::LightType::Directional, true);
	directionalLight->SetRotation({ 30.0f, -80.0 });
	directionalLight->directionalShadowDistance = 25.0f;

	// Tilemap
	tileMap = CreateGameObject()->AddComponent<TileMap>();
	tileMap->ReadFile("./assets/levels/level1.txt");

	// Camera
	auto camera = CreateGameObject()->AddComponent<EVA::Camera>();
	camera->transform->SetPosition({ 0.0f, 3.0f, -2.0f });
	EVA::Application::mainCamera = camera;

	// Pacman
	pacman = CreateGameObject()->AddComponent<Pacman>(tileMap);

	// Shadow
	const auto shadow = CreateGameObject()->AddComponent<GhostShadow>(this);
	ghosts.push_back(shadow);

	//Speedy
	const auto speedy = CreateGameObject()->AddComponent<GhostSpeedy>(this);
	ghosts.push_back(speedy);

	// Bashful
	const auto bashful = CreateGameObject()->AddComponent<GhostBashful>(this, shadow);
	ghosts.push_back(bashful);

	// Pokey
	const auto pokey = CreateGameObject()->AddComponent<GhostPokey>(this);
	ghosts.push_back(pokey);
}

void Game::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
}