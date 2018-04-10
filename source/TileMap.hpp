#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "EVA.hpp"
#include "EVA/Graphics.hpp"

enum TileType
{
	Floor, Wall, Teleporter, GhostFloor, GhostDoor
};

class TileMap : public EVA::Component
{
	int m_Width;
	int m_Height;

	std::vector<std::vector<TileType>> m_Tiles;

	std::map<std::string, std::shared_ptr<EVA::Mesh>> m_MeshMap;
	std::shared_ptr<EVA::Material> m_Material;

	std::vector<glm::ivec2> teleporters;
	std::map<unsigned  int, glm::ivec2> teleporterMap;

	std::map<unsigned int, glm::ivec2> uniqueTiles;

public:

	explicit TileMap(EVA::GameObject *gameObject);

	void ReadFile(const char *path);

	inline int Width() const { return m_Width; }

	inline int Height() const { return m_Height; }

	glm::vec3 GetUniqueTilePosition(unsigned int tile);

	glm::ivec2 TileIndex(glm::vec3 worldPosition) const;

	TileType GetTileType(glm::ivec2 tileIndex);
	TileType GetTileType(glm::vec3 worldPosition);

private:

	void CreateMesh();
};