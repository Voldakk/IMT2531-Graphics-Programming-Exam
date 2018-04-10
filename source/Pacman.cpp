#include "Pacman.hpp"

#include <utility>

#include "EVA/Input.hpp"
#include "glm/gtx/compatibility.hpp"
#include <iostream>

Pacman::Pacman(EVA::GameObject* gameObject, std::shared_ptr<TileMap> tileMap) : Component(gameObject), m_TileMap(std::move(tileMap))
{
	// Mesh
	m_Mesh = EVA::Mesh::Load("./assets/models/pacman.obj");

	// Material
	m_Material = std::make_shared<EVA::Material>();
	m_Material->shader = std::make_shared<EVA::StandardShader>();
	m_Material->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/pacman/pacman_diffuse.png");
	m_Material->SetTexture(EVA::TextureType::Specular, "./assets/textures/pacman/pacman_specular.png");
	m_Material->SetTexture(EVA::TextureType::Normal, "./assets/textures/pacman/pacman_normal.png");

	// Mesh renderer
	auto mr = m_GameObject->AddComponent<EVA::MeshRenderer>();
	mr->Set(m_Mesh, m_Material);

	// Default values
	m_CurrentDirection = glm::vec3(0.0f);
	m_InputDirection = Up;
	m_MovementSpeed = 2.0f;

	m_GameObject->transform->SetPosition(m_TileMap->GetUniqueTilePosition('P'));
	m_GameObject->transform->SetScale(glm::vec3(0.8f));
}

void Pacman::Update(const float deltaTime)
{
	// Input
	if (EVA::Input::Key(GLFW_KEY_UP)) // Front
		m_InputDirection = Up;
	if (EVA::Input::Key(GLFW_KEY_DOWN)) // Back
		m_InputDirection = Down;

	if (EVA::Input::Key(GLFW_KEY_RIGHT)) // Right
		m_InputDirection = Right;
	if (EVA::Input::Key(GLFW_KEY_LEFT)) // Left
		m_InputDirection = Left;
	
	switch (m_InputDirection) 
	{ 
	case Up: 
		m_CurrentDirection = { 0.0f, 0.0f, 1.0f };
		m_GameObject->transform->SetOrientation(EVA::YAXIS, 0.0f);
		break;

	case Down: 
		m_CurrentDirection = { 0.0f, 0.0f, -1.0f };
		m_GameObject->transform->SetOrientation(EVA::YAXIS, 180.0f);
		break;

	case Right:
		m_CurrentDirection = { 1.0f, 0.0f, 0.0f };
		m_GameObject->transform->SetOrientation(EVA::YAXIS, -90.0f);
		break;

	case Left: 
		m_CurrentDirection = { -1.0f, 0.0f, 0.0f };
		m_GameObject->transform->SetOrientation(EVA::YAXIS, 90.0f);
		break;

	default: 
		break;
	}
	
	// Move
	m_GameObject->transform->Translate(m_CurrentDirection * m_MovementSpeed * deltaTime);

	std::cout << "Tile: " << m_TileMap->GetTileType(m_GameObject->transform->position) << "\n";
}