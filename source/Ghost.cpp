#include "Ghost.hpp"

#include "EVA/ResourceManagers.hpp"

#include "Game.hpp"

Ghost::Ghost(EVA::GameObject* gameObject, Game* game)
	: Component(gameObject), m_Direction(Direction::Up), m_State(GhostState::Chase), m_Game(game), m_TileMap(game->tileMap), m_Pacman(game->pacman)
{
	const auto shader = EVA::ShaderManager::GetShader("standard");

	m_Model = EVA::ModelManager::LoadModel("./assets/models/ghost.obj");

	// Body
	m_BodyMaterial = std::make_shared<EVA::Material>();
	m_BodyMaterial->shader = shader;
	m_BodyMaterial->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/ghost/ghost_body_diffuse.png");
	m_BodyMaterial->SetTexture(EVA::TextureType::Specular, "./assets/textures/ghost/ghost_body_specular.png");
	m_BodyMaterial->SetTexture(EVA::TextureType::Normal, "./assets/textures/ghost/ghost_body_normal.png");

	auto bodyMr = gameObject->AddComponent<EVA::MeshRenderer>();
	bodyMr->Set(m_Model->GetMesh("ghost_body"), m_BodyMaterial);

	// Eyes
	m_EyeMaterial = std::make_shared<EVA::Material>();
	m_EyeMaterial->shader = shader;
	m_EyeMaterial->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/ghost/ghost_eyes_diffuse.png");
	m_EyeMaterial->SetTexture(EVA::TextureType::Specular, "./assets/textures/ghost/ghost_eyes_specular.png");
	m_EyeMaterial->SetTexture(EVA::TextureType::Normal, "./assets/textures/ghost/ghost_eyes_normal.png");

	auto eyeMr = gameObject->AddComponent<EVA::MeshRenderer>();
	eyeMr->Set(m_Model->GetMesh("ghost_eyes"), m_EyeMaterial);

	Reset();
}

void Ghost::Update(const float deltaTime)
{
	// Get the current tile
	m_CurrentTile = m_TileMap->GetTileIndex(transform->position);

	if (m_State == GhostState::Dead && m_CurrentTile == m_TileMap->GetUniqueTile('G'))
		Reset();

	// If it's time to select a new target tile
	if (m_TargetTile.x == -1)
	{
		m_Direction = ChooseDirection(FindPossibleDirections());
		const auto possibleTarget = m_CurrentTile + DirectionToVector(m_Direction);

		// The possible target is traversable
		if (m_TileMap->GetTileType(possibleTarget) != TileType::Wall)
		{
			// Set the target and direction
			m_TargetTile = possibleTarget;
			m_CurrentDirection = possibleTarget - m_CurrentTile;

			// Rotate pacman to face the way he is moving
			switch (m_Direction)
			{
			case Up:
				transform->SetOrientation(EVA::YAXIS, 0.0f);
				break;
			case Down:
				transform->SetOrientation(EVA::YAXIS, 180.0f);
				break;
			case Right:
				transform->SetOrientation(EVA::YAXIS, 90.0f);
				break;
			case Left:
				transform->SetOrientation(EVA::YAXIS, -90.0f);
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
	if (nextTile == TileType::Wall)
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
		glm::ivec2 teleportDest;
		if (m_TileMap->GetTeleporter(m_TargetTile, teleportDest))
			transform->SetPosition(TileMap::GetTilePosition(teleportDest));
		else
			transform->SetPosition(targetTilePos);


		// Reset the target tile
		m_TargetTile = glm::ivec2(-1);
	}
	else
	{
		transform->Translate(direction * maxDistance);
	}
}

void Ghost::Reset()
{
	SetState(m_Game->CurrentWave().state);

	m_CurrentTile = m_TileMap->GetUniqueTile('G');
	transform->SetPosition(m_TileMap->GetUniqueTilePosition('G'));

	m_Direction = Direction::Up;
	m_CurrentDirection = DirectionToVector(m_Direction);
	transform->SetOrientation(EVA::YAXIS, 0.0f);

	m_TargetTile.x = -1;
}

void Ghost::SetState(const GhostState newState)
{
	m_State = newState;

	if (m_State == GhostState::Dead)
		SetMaterialColor(glm::vec3(0.8f));
	else if (m_State == GhostState::Scatter || m_State == GhostState::Chase)
		SetMaterialColor(m_Color);
}

void Ghost::ResetState()
{
	SetState(m_Game->CurrentWave().state);
}

void Ghost::SetMaterialColor(const glm::vec3 color) const
{
	m_BodyMaterial->tintDiffuse = glm::vec4(color, 1.0f);
}

glm::ivec2 Ghost::DirectionToVector(const Direction direction)
{
	switch (direction)
	{
	case Up:
		return glm::ivec2(0, 1);

	case Down:
		return glm::ivec2(0, -1);

	case Right:
		return glm::ivec2(1, 0);

	case Left:
	default:
		return glm::ivec2(-1, 0);
	}
}

Ghost::Direction Ghost::VectorToDirection(const glm::ivec2 direction)
{
	if (direction.x >= 1 && direction.y == 0)
		return Direction::Right;
	if (direction.x <= -1 && direction.y == 0)
		return Direction::Left;
	if (direction.x == 0 && direction.y >= 1)
		return Direction::Up;
	if (direction.x == 0 && direction.y <= -1)
		return Direction::Down;

	return Direction::Up;
}

std::vector<Ghost::Direction> Ghost::FindPossibleDirections() const
{
	std::vector<Direction> directions;

	for (auto i = 0; i < 4; ++i)
	{
		if (((Direction)i == Direction::Up    && m_Direction == Direction::Down )||
			((Direction)i == Direction::Down  && m_Direction == Direction::Up   )||
			((Direction)i == Direction::Right && m_Direction == Direction::Left )||
			((Direction)i == Direction::Left  && m_Direction == Direction::Right))
			continue;

		const auto direction = DirectionToVector((Direction)i);
		const auto tile = m_TileMap->GetTileType(m_CurrentTile + direction);

		switch (m_State)
		{
		case GhostState::Chase:
		case GhostState::Scatter:
		case GhostState::Frightened:
			switch (tile)
			{
			case TileType::Floor:
			case TileType::Teleporter:
				directions.push_back((Direction)i);
				break;

			default:
				break;
			}
			break;
		case GhostState::Dead:
			if (tile != TileType::Wall)
				directions.push_back((Direction)i);
			break;
		}
	}

	return directions;
}

Ghost::Direction Ghost::ChooseDirection(const std::vector<Ghost::Direction>& directions) const
{
	const auto currentTile = m_TileMap->GetTileType(m_CurrentTile);

	switch (currentTile)
	{
	case TileType::Floor:
		switch (m_State)
		{
		case Chase:
			return ChooseChaseTarget(directions);

		case Scatter:
			return DirectionToTarget(directions, m_ScatterTile);

		case Frightened:
			if (directions.empty())
				return Direction::Up;
			else
				return directions[rand() % directions.size()];

		case Dead:
		default:
			return DirectionToTarget(directions, m_TileMap->GetUniqueTile('G'));
		}
		

	case TileType::GhostSpawn:
	case TileType::GhostFloor:
	case TileType::GhostDoor:
	default:
		switch (m_State)
		{
		case GhostState::Chase:
		case GhostState::Scatter:
		case GhostState::Frightened:
			return Direction::Up;

		case GhostState::Dead:
		default:
			return DirectionToTarget(directions, m_TileMap->GetUniqueTile('G'));
		}
	}

	
}

Ghost::Direction Ghost::DirectionToTarget(const std::vector<Ghost::Direction>& directions, const glm::ivec2 target) const
{
	auto minDist = 100000.0f;
	auto minDir = Direction::Up;

	for (auto direction : directions)
	{
		const auto tile = m_CurrentTile + DirectionToVector(direction);

		const auto dist = glm::distance(glm::vec2(tile.x, tile.y), glm::vec2(target.x, target.y));

		if (dist < minDist)
		{
			minDist = dist;
			minDir = direction;
		}
	}

	return minDir;
}

void Ghost::SetColor(const glm::vec3 newColor)
{
	m_Color = newColor;
	m_BodyMaterial->tintDiffuse = glm::vec4(newColor, 1.0f);
}
