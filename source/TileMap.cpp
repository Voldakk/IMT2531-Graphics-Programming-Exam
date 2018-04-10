#include "TileMap.hpp"

#include <fstream>

TileMap::TileMap(EVA::GameObject *gameObject) : Component(gameObject),
m_Width(0), m_Height(0)
{
	// Load meshes
	auto meshes = EVA::Mesh::LoadMultiple("./assets/models/tile.obj");

	for (const auto &mesh : meshes)
	{
		mesh->isStatic = true;
		m_MeshMap[mesh->name] = mesh;
	}

	// Material
	m_Material = std::make_shared<EVA::Material>();
	m_Material->enableInstancing = true;
	m_Material->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/Cement 1/Cement 1_Dif.jpg");
	m_Material->SetTexture(EVA::TextureType::Specular, "./assets/textures/Cement 1/Cement 1_Ref.jpg");
	m_Material->SetTexture(EVA::TextureType::Normal, "./assets/textures/Cement 1/Cement 1_Nor.jpg");
	m_Material->shader = std::make_shared<EVA::StandardInstancedShader>();
}

void TileMap::ReadFile(const char *path)
{
	// Clear the map
	m_Tiles.clear();

	//	Opens the file
	std::ifstream mapFile;
	mapFile.open(path);

	// If the file was opened correctly
	if (mapFile.good())
	{
		//	Reads the width and height
		mapFile >> m_Width;
		mapFile >> m_Height;
		mapFile.ignore();

		// Make room for teleporters
		for (auto i = 0; i < 10; ++i)
		{
			teleporters.emplace_back(-1, -1);
		}

		// For each row
		for (auto y = 0; y < m_Height; y++)
		{
			std::vector<TileType> row;

			for (auto x = 0; x < m_Width; x++)
			{
				const auto t = mapFile.get();

				switch (t)
				{
				case 'S':
				case 'B':
				case 'C':
				case 'I':
					uniqueTiles[t] = glm::ivec2(x, y);
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
					teleporters[t] = glm::ivec2(x, y);
					break;

				case '.':
					row.push_back(TileType::Floor);
					break;

				default:
					row.push_back(TileType::Floor);
					uniqueTiles[t] = glm::ivec2(x, y);
					break;
				}

				mapFile.ignore();
			}

			m_Tiles.push_back(row);
		}

		CreateMesh();
	}
	else
	{
		printf("\nFailed to read level %s\n", path);
	}
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
				config += 1;
			if (m_Tiles[y][x + 1] == TileType::Wall)
				config += 2;
			if (m_Tiles[y + 1][x] == TileType::Wall)
				config += 4;
			if (m_Tiles[y + 1][x + 1] == TileType::Wall)
				config += 8;

			// Get the mesh
			const auto mesh = m_MeshMap[std::to_string(config)];

			// If we found a mesh
			if (mesh != nullptr)
			{
				// Create a GameObject and a MeshRenderer to display the mesh
				auto go = m_GameObject->GetScene()->CreateGameObject();
				go->transform->SetPosition({ x, 0.0f, y });
				go->transform->SetScale(glm::vec3(0.5f));
				auto mr = go->AddComponent<EVA::MeshRenderer>();
				mr->Set(mesh, m_Material);
			}
		}
	}
}

glm::vec3 TileMap::GetUniqueTilePosition(const unsigned int tile)
{
	return glm::vec3(uniqueTiles[tile].x - 0.5f, 0.0f, uniqueTiles[tile].y - 0.5f);
}