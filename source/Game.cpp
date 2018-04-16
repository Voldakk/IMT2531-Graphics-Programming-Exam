#include "Game.hpp"

#include "EVA/Graphics.hpp"
#include "EVA/SceneManager.hpp"
#include "EVA/ResourceManagers.hpp"

#include "TileMap.hpp"
#include "GhostVariations.hpp"
#include "Pacman.hpp"

#include <iostream>
#include "Pellet.hpp"

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
	directionalLight->SetRotation({ 60.0f, -80.0 });
	directionalLight->directionalShadowDistance = 25.0f;

	// Tilemap
	Pellet::Init();
	tileMap = CreateGameObject()->AddComponent<TileMap>(this);
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

	// Waves
	m_Waves.push_back({ GhostState::Scatter, 7.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 7.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 5.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 5.0f });
	m_Waves.push_back({ GhostState::Chase,   0.0f });

	m_WaveTimer = 0.0f;
	m_CurrentWave = 0;
	for (auto& ghost : ghosts)
	{
		ghost->SetState(CurrentWave().state);
	}
	std::cout << "New wave: " << (CurrentWave().state == GhostState::Scatter ? "Scatter" : "Chase") << ", time: " << CurrentWave().time << "\n";

	// Score
	m_Score = 0;
}

void Game::Update(const float deltaTime)
{
	Scene::Update(deltaTime);

	m_WaveTimer += deltaTime;
	if(m_WaveTimer >= CurrentWave().time && m_CurrentWave < m_Waves.size() - 1)
	{
		m_CurrentWave++;
		m_WaveTimer = 0.0f;
		for (auto& ghost : ghosts)
		{
			ghost->SetState(CurrentWave().state);
		}

		std::cout << "New wave: " << (CurrentWave().state == GhostState::Scatter ? "Scatter" : "Chase") << ", time: " << CurrentWave().time << "\n";
	}
}

void Game::AddScore(const unsigned int amount)
{
	m_Score += amount;
	std::cout << "Score: " << m_Score << "\n";
}
