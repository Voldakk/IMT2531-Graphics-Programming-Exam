#pragma once

#include <vector>
#include <string>

#include "Mesh.hpp"

class OBJLoader
{
public:

	static std::vector<std::shared_ptr<Mesh>> Load(const char * path);

private:

	static std::shared_ptr<Mesh> CreateMesh(
		std::string name, 
		std::vector<glm::vec3> tmpVert,
		std::vector<glm::vec2> tmpUv,
		std::vector<glm::vec3> tmpNorms,
		std::vector<unsigned int> finVertIdx,
		std::vector<unsigned int> finUvIdx,
		std::vector<unsigned int> finNormIdx
	);
};