#pragma once

#include <string>
#include <memory>

class Mesh;
class Shader;
class Material;
class Transform;

class Skybox
{
public:

	unsigned int texture = -1;

	std::unique_ptr<Mesh> mesh;
	std::unique_ptr<Material> material;
	std::unique_ptr<Transform> transform;

	Skybox(const std::string& folderPath, const std::string& fileType);
	~Skybox();

	void Render() const;
};