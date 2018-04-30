#include "Game.hpp"

#include <iostream>

#include "EVA/Graphics.hpp"
#include "EVA/SceneManager.hpp"
#include "EVA/ResourceManagers.hpp"
#include "EVA/Input.hpp"

#include "../EVA/Components/FreeLook.hpp"
#include "../EVA/Components/FollowTarget.hpp"

#include "GhostVariations.hpp"
#include "PauseMenu.hpp"
#include "GameOverScene.hpp"
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

	// Waves
	m_Waves.push_back({ GhostState::Scatter, 7.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 7.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 5.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 5.0f });
	m_Waves.push_back({ GhostState::Chase,   0.0f });

	for (auto& ghost : ghosts)
	{
		ghost->SetState(CurrentWave().state);
	}
	std::cout << "New wave: " << (CurrentWave().state == GhostState::Scatter ? "Scatter" : "Chase") << ", time: " << CurrentWave().time << "\n";

	// Tilemap
	Pellet::ResetCount();
	tileMap = CreateGameObject()->AddComponent<TileMap>(this);
	tileMap->ReadFile("./assets/levels/level1.txt");

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

	// Camera
	m_Camera = CreateGameObject()->AddComponent<EVA::Camera>();
	m_Camera->transform->SetPosition(pacman->transform->position);
	EVA::Application::SetMainCamera(m_Camera);

	m_FreeCamera = m_Camera->gameObject->AddComponent<EVA::FreeLook>();
	m_FreeCamera->wasd = false;

	m_ChaseCamera = m_Camera->gameObject->AddComponent<EVA::FollowTarget>();
	m_ChaseCamera->target = pacman->gameObject->transform.get();
	m_ChaseCamera->offset = glm::vec3(0.0f, 4.0f, -2.0f);

	// UI
	EVA::Input::SetCursorMode(EVA::Input::Disabled);

	m_ScoreLabel = CreateUiElement<EVA::Label>("Score: " + std::to_string(m_Score));
	m_ScoreLabel->SetAnchorAndPivot(-1.0f, 1.0f); // Top left
	m_ScoreLabel->SetOffsetFromAnchor(0.05f);	  // Padding 

	m_ExtraLivesLabel = CreateUiElement<EVA::Label>("Extra lives: " + std::to_string(m_ExtraLives));
	m_ExtraLivesLabel->SetAnchorAndPivot(-1.0f, 1.0f); // Top left
	m_ExtraLivesLabel->SetOffset(0.05f, -0.2f);		   // Moved down a bit

	m_CameraModeLabel = CreateUiElement<EVA::Label>("Camera mode");
	m_CameraModeLabel->SetAnchorAndPivot(-1.0f, -1.0f); // Bottom left
	m_CameraModeLabel->SetOffsetFromAnchor(0.05f);	    // Padding 

	// Set camera mode
	m_CurrentCameraMode = TopDown;
	UpdateCameraMode();
}

void Game::Update(const float deltaTime)
{
	// Pause menu
	if (EVA::Input::KeyDown(EVA::Input::Escape) && !m_IsPaused)
	{
		m_IsPaused = true;
		EVA::SceneManager::CreateScene<PauseMenu>(this);
	}

	if (m_IsPaused)
		return;

	// Update scene
	Scene::Update(deltaTime);

	// Camera mode
	if (EVA::Input::KeyDown(EVA::Input::C))
	{
		m_CurrentCameraMode++;
		if (m_CurrentCameraMode == Last)
			m_CurrentCameraMode = 0;

		UpdateCameraMode();
	}

	// Waves
	m_WaveTimer += deltaTime;
	if(m_WaveTimer >= CurrentWave().time && m_CurrentWave < m_Waves.size() - 1)
	{
		m_CurrentWave++;
		m_WaveTimer = 0.0f;
		for (auto& ghost : ghosts)
		{
			if (ghost->state == GhostState::Dead || ghost->state == GhostState::Frightened)
				continue;

			ghost->SetState(CurrentWave().state);
		}

		std::cout << "New wave: " << (CurrentWave().state == GhostState::Scatter ? "Scatter" : "Chase") << ", time: " << CurrentWave().time << "\n";
	}

	// Energizer
	if(m_ActiveEnergizer)
	{
		m_EnergizerTimer -= deltaTime;
		if(m_EnergizerTimer <= 0)
		{
			m_ActiveEnergizer = false;
			m_Time = 0.0f;

			for (auto& ghost : ghosts)
			{
				if (ghost->state == GhostState::Dead)
					continue;

				ghost->ResetState();
			}
		}
		else
		{
			m_Time += deltaTime;

			for (auto& ghost : ghosts)
			{
				if (ghost->state != GhostState::Frightened)
					continue;

				if(m_EnergizerTimer <= 2.0f)
					ghost->SetMaterialColor(glm::vec3(glm::vec2(glm::sin(m_Time * 10.0f) + 0.5f), 1.0f));
				else
					ghost->SetMaterialColor(glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
	}

	// Pacman vs ghosts
	for (auto& ghost : ghosts)
	{
		if (ghost->state == GhostState::Dead)
			continue;

		if(ghost->currentTile == pacman->currentTile)
		{
			if(ghost->state == GhostState::Frightened)
			{
				ghost->SetState(GhostState::Dead);
				AddScore(1000);
			}
			else
			{
				if (m_ExtraLives == 0)
				{
					EVA::SceneManager::ChangeScene<GameOverScene>(false, m_Score);
				}
				else
				{
					m_ExtraLives--;
					m_ExtraLivesLabel->SetText("Extra lives: " + std::to_string(m_ExtraLives));
					Reset();
				}
			}
		}
	}
}

Game::Wave Game::CurrentWave()
{
	if(m_CurrentWave < m_Waves.size())
		return m_Waves[m_CurrentWave];

	return {};
}

void Game::AddScore(const unsigned int amount)
{
	m_Score += amount;
	m_ScoreLabel->SetText("Score: " + std::to_string(m_Score));
}

void Game::ActivateEnergizer(const float time)
{
	m_EnergizerTimer = time;
	m_ActiveEnergizer = true;

	for (auto& ghost : ghosts)
	{
		if (ghost->state == GhostState::Dead)
			continue;

		ghost->SetState(GhostState::Frightened);
	}
}

void Game::Unpause()
{
	m_IsPaused = false;
	EVA::Input::SetCursorMode(EVA::Input::Disabled);
}

void Game::Reset()
{
	m_CurrentWave = 0;
	m_WaveTimer = 0.0f;
	std::cout << "New wave: " << (CurrentWave().state == GhostState::Scatter ? "Scatter" : "Chase") << ", time: " << CurrentWave().time << "\n";

	for (auto ghost : ghosts)
	{
		ghost->Reset();
	}

	pacman->Reset();

	m_ActiveEnergizer = false;
}

void Game::UpdateCameraMode()
{
	std::string cameraModeName;

	switch (m_CurrentCameraMode)
	{
	case Free:
		cameraModeName = "Free";
		pacman->inputMode = Pacman::Global;

		m_ChaseCamera->SetActive(false);
		m_FreeCamera->SetActive(true);
		break;

	case Chase:
		cameraModeName = "Chase";
		pacman->inputMode = Pacman::Local;

		m_ChaseCamera->SetActive(true);
		m_FreeCamera->SetActive(false);
		break;

	case TopDown:
	default:
		cameraModeName = "Top-Down";
		pacman->inputMode = Pacman::Global;

		m_ChaseCamera->SetActive(false);
		m_FreeCamera->SetActive(false);

		m_Camera->transform->SetPosition({ -tileMap->width / 2.0f, 20.0f, tileMap->height / 2.0f });
		m_Camera->transform->SetOrientation(EVA::XAXIS, -90.0f);
		break;
	}

	m_CameraModeLabel->SetText("Camera mode: " + cameraModeName);
}

void Game::Win()
{
	EVA::SceneManager::ChangeScene<GameOverScene>(true, m_Score);
}
