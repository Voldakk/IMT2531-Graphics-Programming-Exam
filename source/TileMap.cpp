#include "TileMap.hpp"

#include <fstream>

TileMap::TileMap(EVA::GameObject *gameObject) : Component(gameObject),
                                                m_Width(0), m_Height(0)
{
    // Load meshes
    auto meshes = EVA::Mesh::LoadMultiple("./assets/tiles/tile.obj");

    for (const auto &mesh : meshes)
    {
        m_MeshMap[mesh->m_Name] = mesh;
    }

    // Shaders
    const auto standardInstancedShader = std::make_shared<EVA::StandardShader>();

    // Materials
    m_Material = std::make_shared<EVA::Material>();
    m_Material->AddTexture(EVA::TextureType::Diffuse, "./assets/container2.png");
    m_Material->AddTexture(EVA::TextureType::Specular, "./assets/container2_specular.png");
    m_Material->m_Shader = standardInstancedShader;
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

        // For each row
        for (auto y = 0; y < m_Height; y++)
        {
            std::vector<TileType> row;

            for (auto x = 0; x < m_Width; x++)
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

            m_Tiles.push_back(row);
        }

        CreateMesh();
    } else
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
                go->GetTransform()->SetPosition({x * 2.0f, 0.0f, y * 2.0f});

                auto mr = go->AddComponent<EVA::MeshRenderer>();
                mr->Set(mesh, m_Material);
            }
        }
    }
}