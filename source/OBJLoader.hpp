#pragma once

#include <vector>

#include "Mesh.hpp"

class OBJLoader
{
public:
	static bool Load(const char * path, std::vector<Vertex> & vertices);
};

