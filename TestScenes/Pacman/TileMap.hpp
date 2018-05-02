#pragma once

#include <map>
#include <vector>
#include <memory>

#include "EVA.hpp"
#include "EVA/Graphics.hpp"

#include "Ghost.hpp"

class Game;

enum TileType
{
	Floor, Wall, GhostFloor, GhostDoor, GhostSpawn
};

class TileMap : public EVA::Component
{
	int m_Width = 0;
	int m_Height = 0;

	std::vector<std::vector<TileType>> m_Tiles;

	std::shared_ptr<EVA::Model> m_Model;
	std::shared_ptr<EVA::Material> m_Material;

	std::vector<glm::ivec2> m_Teleporters;

	std::map<unsigned int, glm::ivec2> m_UniqueTiles;

	

public:

	Game * game;

	const int& width = m_Width;
	const int& height = m_Height;

	void Start() override;

	/**
	 * \brief Reads a level from the specified file
	 * \param path The path to the file
	 */
	void ReadFile(const char *path);

	/**
	 * \brief Gets the world position of a tile with the given symbol
	 * \param tile The tile symbol
	 * \return The world position
	 */
	glm::vec3 GetUniqueTilePosition(const unsigned int tile);

	/**
	 * \brief Gets the tile index of a tile with the given symbol
	 * \param tile The tile symbol
	 * \return The tile index
	 */
	glm::ivec2 GetUniqueTile(const unsigned tile);

	/**
	 * \brief Gets the corresponding tile index from a world psoition
	 * \param worldPosition The world position
	 * \return The tile index
	 */
	glm::ivec2 GetTileIndex(glm::vec3 worldPosition) const;

	/**
	 * \brief Gets the tile type of the tile at the given index
	 * \param tileIndex The tile index
	 * \return The tile type
	 */
	TileType GetTileType(glm::ivec2 tileIndex);

	/**
	 * \brief gets the tile type of the tile located in the given world position
	 * \param worldPosition The world position
	 * \return The tile type
	 */
	TileType GetTileType(glm::vec3 worldPosition);

	/**
	 * \brief Gets the world position of the tile in the given index
	 * \param tileIndex The tile index
	 * \return The world position
	 */
	static glm::vec3 GetTilePosition(glm::ivec2 tileIndex);

	bool GetTeleporter(glm::ivec2 tileIndex, glm::ivec2& destinationIndex);

private:

	/**
	 * \brief Creates all the meshes representing the tile map
	 */
	void CreateMesh();
};