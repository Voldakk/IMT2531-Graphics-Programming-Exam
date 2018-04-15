#include "TileMap.hpp"

#include <fstream>

#include "EVA/ResourceManagers.hpp"

TileMap::TileMap(EVA::GameObject *gameObject) 
	: Component(gameObject), m_Width(0), m_Height(0)
{
	// Load meshes
	m_Model = EVA::ModelManager::LoadModel("./assets/models/tile.obj");


	for (unsigned int i = 0; i < m_Model->MeshCount(); ++i)
	{
		m_Model->GetMesh(i)->isStatic = true;
	}

	// Material
	m_Material = std::make_shared<EVA::Material>();
	m_Material->enableInstancing = true;
	m_Material->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/Cement 1/Cement 1_Dif.jpg");
	m_Material->SetTexture(EVA::TextureType::Specular, "./assets/textures/Cement 1/Cement 1_Ref.jpg");
	m_Material->SetTexture(EVA::TextureType::Normal, "./assets/textures/Cement 1/Cement 1_Nor.jpg");
	m_Material->shader = EVA::ShaderManager::GetShader("standard_instanced");
}

void TileMap::ReadFile(const char *path)
{
	// Clear the map
	m_Tiles.clear();

	//	Opens the file
	std::ifstream mapFile;
	mapFile.open(path);

	std::vector<std::vector<int>> level;

	// Read the level file
	if (mapFile.good())
	{
		//	Reads the width and height
		mapFile >> m_Width;
		mapFile >> m_Height;
		mapFile.ignore();

		level.reserve(m_Height);

		// For each row
		for (auto y = 0; y < m_Height; y++)
		{
			std::vector<int> row;
			row.reserve(m_Width);

			for (auto x = 0; x < m_Width; x++)
			{
				row.push_back(mapFile.get());
				mapFile.ignore();
			}

			level.push_back(row);
		}
	}
	else
	{
		printf("\nFailed to read level %s\n", path);
		return;
	}

	// Raverese the y
	for (auto y = 0; y < m_Height / 2; ++y)
	{
		std::swap(level[y], level[m_Height - 1 - y]);
	}

	// Make room for teleporters
	for (auto i = 0; i < 10; ++i)
	{
		m_Teleporters.emplace_back(-1, -1);
	}

	m_Tiles.resize(m_Height);

	// For each row
	for (auto y = 0; y < m_Height; y++)
	{
		std::vector<TileType> row;

		for (auto x = 0; x < m_Width; x++)
		{
			const auto t = level[y][x];
			m_UniqueTiles[t] = glm::ivec2(x, y);

			switch (t)
			{
			case 'D':
				row.push_back(TileType::GhostDoor);
				break;

			case 'H':
				row.push_back(TileType::GhostFloor);
				break;

			case 'G':
				row.push_back(TileType::GhostSpawn);
				break;

			case 'S':
			case 'B':
			case 'C':
			case 'I':
			case '#':
				row.push_back(TileType::Wall);
				break;

			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				row.push_back(TileType::Teleporter);
				m_Teleporters[t] = glm::ivec2(x, y);
				break;

			case '.':
				row.push_back(TileType::Floor);
				break;

			default:
				row.push_back(TileType::Floor);
				break;
			}

			mapFile.ignore();
		}

		m_Tiles[y] = row;
	}

	CreateMesh();
}

void TileMap::CreateMesh()
{
	for (auto y = 0; y < m_Height - 1; ++y)
	{
		for (auto x = 0; x < m_Width - 1; ++x)
		{
			// Find the config
			auto config = 0;

			if (m_Tiles[y][x] == TileType::Wall)
				config += 4;
			if (m_Tiles[y][x + 1] == TileType::Wall)
				config += 8;
			if (m_Tiles[y + 1][x] == TileType::Wall)
				config += 1;
			if (m_Tiles[y + 1][x + 1] == TileType::Wall)
				config += 2;

			// Get the mesh
			const auto mesh = m_Model->GetMesh(std::to_string(config));

			// If we found a mesh
			if (mesh != nullptr)
			{
				// Create a GameObject and a MeshRenderer to display the mesh
				auto go = scene->CreateGameObject();
				go->transform->SetPosition({ x+1, 0.0f, y+1 });
				go->transform->SetScale(glm::vec3(0.5f));
				auto mr = go->AddComponent<EVA::MeshRenderer>();
				mr->Set(mesh, m_Material);
			}
		}
	}
}

glm::vec3 TileMap::GetUniqueTilePosition(const unsigned int tile)
{
	return glm::vec3(m_UniqueTiles[tile].x + 0.5f, 0.0f, m_UniqueTiles[tile].y + 0.5f);
}

glm::ivec2 TileMap::GetUniqueTile(const unsigned tile)
{
	return m_UniqueTiles[tile];
}

glm::ivec2 TileMap::GetTileIndex(const glm::vec3 worldPosition) const
{
	if (worldPosition.x < 0.0f || worldPosition.x > m_Width || worldPosition.z < 0.0f || worldPosition.z > m_Height)
		return glm::ivec2(-1);

	return glm::ivec2((int)worldPosition.x, (int)worldPosition.z);
}

TileType TileMap::GetTileType(const glm::ivec2 tileIndex)
{
	if (tileIndex.x < 0 || tileIndex.x >= m_Width || tileIndex.y < 0 || tileIndex.y >= m_Height)
		return TileType::Wall;

	return m_Tiles[tileIndex.y][tileIndex.x];
}

TileType TileMap::GetTileType(const glm::vec3 worldPosition)
{
	return GetTileType(GetTileIndex(worldPosition));
}

glm::vec3 TileMap::GetTilePosition(const glm::ivec2 tileIndex) const
{
	return glm::vec3(tileIndex.x + 0.5f, 0.0f, tileIndex.y + 0.5f);
}
