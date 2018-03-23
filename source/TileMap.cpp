#include "TileMap.hpp"

#include <fstream>

#include "EVA-Engine/GameObject.hpp"
#include "EVA-Engine/MeshRenderer.hpp"
#include "EVA-Engine/Scene.hpp"

TileMap::TileMap(GameObject* gameObject) : Component(gameObject)
{
	width = 0;
	height = 0;

	// Load meshes
	auto meshes = Mesh::LoadMultiple("./assets/tiles/tile.obj");

	for (const auto& mesh : meshes)
	{
		meshMap[mesh->name] = mesh;
	}

	// Shaders
	const auto standardInstancedShader = std::make_shared<StandardInstancedShader>();

	// Materials
	material = std::make_shared<Material>();
	material->AddTexture(TextureType::Diffuse, "./assets/uv.png");
	material->shader = standardInstancedShader;
	material->enableInstancing = true;
}

void TileMap::ReadFile(const char* path)
{
	// Clear the map
	tiles.clear();

	//	Opens the file
	std::ifstream mapFile;
	mapFile.open(path);

	// If the file was opened correctly
	if (mapFile.good())
	{
		//	Reads the width and height
		mapFile >> width;
		mapFile >> height;
		mapFile.ignore();

		// For each row
		for (auto y = 0; y < height; y++)
		{
			std::vector<TileType> row;

			for (auto x = 0; x < width; x++)
			{
				char c = mapFile.get();

				switch (c)
				{
				case '#':
				case 'S':
				case 'B':
				case 'C':
				case 'I':
					row.push_back(TileType::Wall);
					break;
				default:
					row.push_back(TileType::Floor);
					break;
				}

				mapFile.ignore();
			}

			tiles.push_back(row);
		}
		int a = 0;
		a++;

		CreateMesh();
	}
	else
	{
		printf("\nFailed to read level %s\n", path);
	}
}

void TileMap::CreateMesh()
{
	for (auto y = 0; y < height - 1; ++y)
	{
		for (auto x = 0; x < width - 1; ++x)
		{
			// Find the config
			auto config = 0;

			if (tiles[y][x] == TileType::Wall)
				config += 1;
			if (tiles[y][x + 1] == TileType::Wall)
				config += 2;
			if (tiles[y + 1][x] == TileType::Wall)
				config += 4;
			if (tiles[y + 1][x + 1] == TileType::Wall)
				config += 8;

			// Get the mesh
			const auto mesh = meshMap[std::to_string(config)];

			// If we found a mesh
			if (mesh != nullptr)
			{
				// Create a GameObject and a MeshRenderer to display the mesh
				auto go = gameObject->scene->CreateGameObject();
				go->transform->SetPosition({ x * 2.0f, 0.0f, y * 2.0f });

				auto mr = go->AddComponent<MeshRenderer>();
				mr->Set(mesh, material);
			}
		}
	}
}