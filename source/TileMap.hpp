#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "EVA.hpp"
#include "EVA/Graphics.hpp"

enum TileType
{
    Floor, Wall
};

class TileMap : public EVA::Component
{

private:

    int m_Width;
    int m_Height;

    std::vector<std::vector<TileType>> m_Tiles;

    std::map<std::string, std::shared_ptr<EVA::Mesh>> m_MeshMap;
    std::shared_ptr<EVA::Material> m_Material;

public:

    explicit TileMap(EVA::GameObject *gameObject);

    void ReadFile(const char *path);

    inline int Width() const
    { return m_Width; }

    inline int Height() const
    { return m_Height; }

private:

    void CreateMesh();
};
