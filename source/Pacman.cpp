#include "Pacman.hpp"

#include "EVA/Input.hpp"
#include "EVA/ResourceManagers.hpp"

Pacman::Pacman(EVA::GameObject* gameObject, TileMap* tileMap) : Component(gameObject), m_TileMap(tileMap)
{
	// Mesh
	m_Model = EVA::ModelManager::LoadModel("./assets/models/pacman.obj");

	// Material
	m_Material = std::make_shared<EVA::Material>();
	m_Material->shader = EVA::ShaderManager::GetShader("standard");
	m_Material->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/pacman/pacman_diffuse.png");
	m_Material->SetTexture(EVA::TextureType::Specular, "./assets/textures/pacman/pacman_specular.png");
	m_Material->SetTexture(EVA::TextureType::Normal, "./assets/textures/pacman/pacman_normal.png");

	// Mesh renderer
	auto mr = gameObject->AddComponent<EVA::MeshRenderer>();
	mr->Set(m_Model->GetMesh(0), m_Material);

	mr = gameObject->AddComponent<EVA::MeshRenderer>();
	mr->Set(m_Model->GetMesh(1), m_Material);

	// Default values
	Reset();
}

void Pacman::Update(const float deltaTime)
{
	// Get the current tile
	m_CurrentTile = m_TileMap->GetTileIndex(transform->position);

	// Input
	if (EVA::Input::Key(EVA::Input::Up))
		m_InputDirection = Direction::Up;
	if (EVA::Input::Key(EVA::Input::Down))
		m_InputDirection = Direction::Down;

	if (EVA::Input::Key(EVA::Input::Right))
		m_InputDirection = Direction::Right;
	if (EVA::Input::Key(EVA::Input::Left))
		m_InputDirection = Direction::Left;

	// If it's time to select a new target tile
	if (m_TargetTile.x == -1)
	{
		glm::ivec2 possibleTarget;

		// Select a possible target based on the player input
		switch (m_InputDirection)
		{
		case Up:
			possibleTarget = m_CurrentTile + glm::ivec2(0, 1);
			break;

		case Down: 
			possibleTarget = m_CurrentTile + glm::ivec2(0, -1);
			break;

		case Right: 
			possibleTarget = m_CurrentTile + glm::ivec2(1, 0);
			break;

		case Left: 
			possibleTarget = m_CurrentTile + glm::ivec2(-1, 0);
			break;

		default: 
			break;;
		}

		// The possible target is traversable by pacman
		if (m_TileMap->GetTileType(possibleTarget) == TileType::Floor)
		{
			// Set the target and direction
			m_TargetTile = possibleTarget;
			m_CurrentDirection = possibleTarget - m_CurrentTile;

			// Rotate pacman to face the way he is moving
			switch (m_InputDirection) 
			{ 
			case Up: 
				transform->SetOrientation(EVA::YAXIS, 0.0f);
				break;
			case Down: 
				transform->SetOrientation(EVA::YAXIS, 180.0f);
				break;
			case Right: 
				transform->SetOrientation(EVA::YAXIS, -90.0f);
				break;
			case Left: 
				transform->SetOrientation(EVA::YAXIS, 90.0f);
				break;
			}
		}
		// If it's not traversable
		else
		{
			// Try to continue in the current direction
			m_TargetTile = m_CurrentTile + m_CurrentDirection;
		}
	}

	// Return if the target tile isn't traversable
	const auto nextTile = m_TileMap->GetTileType(m_TargetTile);
	if (nextTile != TileType::Floor)
	{
		m_TargetTile.x = -1;
		return;
	}

	// Find the direction and distance pacman should move
	const auto targetTilePos = TileMap::GetTilePosition(m_TargetTile);
	const auto direction = glm::normalize(targetTilePos - transform->position);
	const auto distToTile = glm::distance(transform->position, targetTilePos);

	// The maximum distance pacman can move this frame
	const auto maxDistance = m_MovementSpeed * deltaTime;

	// Make sure to not overshoot
	if (maxDistance > distToTile)
	{
		transform->SetPosition(targetTilePos);


		// Reset the target tile
		m_TargetTile = glm::ivec2(-1);
	}
	else
	{
		transform->Translate(direction * maxDistance);
	}	
}

void Pacman::Reset()
{
	m_TargetTile.x = -1;
	m_MovementSpeed = 2.0f;

	m_InputDirection = Right;
	m_CurrentDirection = glm::ivec2(1, 0);
	transform->SetOrientation(EVA::YAXIS, -90.0f);

	transform->SetPosition(m_TileMap->GetUniqueTilePosition('P'));
	transform->SetScale(glm::vec3(0.8f));
}
