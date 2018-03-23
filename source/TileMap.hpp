#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>


#include "EVA-Engine/Component.hpp"
#include "EVA-Engine/Mesh.hpp"
#include "EVA-Engine/Material.hpp"

class TileMap : public Component
{
public:

	enum TileType { Floor, Wall };

	explicit TileMap(GameObject * gameObject);

	void ReadFile(const char * path);

	int Width() const
	{
		return width;
	}

	int Height() const
	{
		return height;
	}

private:

	int width;
	int height;

	std::vector<std::vector<TileType>> tiles;

	std::map<std::string, std::shared_ptr<Mesh>> meshMap;
	std::shared_ptr<Material> material;
	
	void CreateMesh();
};
