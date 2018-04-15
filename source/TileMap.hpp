#pragma once

#include <map>
#include <vector>
#include <memory>

#include "EVA.hpp"
#include "EVA/Graphics.hpp"
#include "Ghost.hpp"

enum TileType
{
	Floor, Wall, Teleporter, GhostFloor, GhostDoor, GhostSpawn
};

class TileMap : public EVA::Component
{
	int m_Width;
	int m_Height;

	std::vector<std::vector<TileType>> m_Tiles;

	std::shared_ptr<EVA::Model> m_Model;
	std::shared_ptr<EVA::Material> m_Material;

	std::vector<glm::ivec2> m_Teleporters;
	std::map<unsigned  int, glm::ivec2> m_TeleporterMap;

	std::map<unsigned int, glm::ivec2> m_UniqueTiles;

public:

	const int& width = m_Width;
	const int& height = m_Height;

	explicit TileMap(EVA::GameObject *gameObject);

	void ReadFile(const char *path);

	glm::vec3 GetUniqueTilePosition(unsigned int tile);
	glm::ivec2 GetUniqueTile(unsigned int tile);

	glm::ivec2 GetTileIndex(glm::vec3 worldPosition) const;

	TileType GetTileType(glm::ivec2 tileIndex);
	TileType GetTileType(glm::vec3 worldPosition);

	glm::vec3 GetTilePosition(glm::ivec2 tileIndex) const;

private:

	void CreateMesh();
};