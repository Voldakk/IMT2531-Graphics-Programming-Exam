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

	unsigned int texture;

	std::unique_ptr<Mesh> mesh;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Material> material;
	std::unique_ptr<Transform> transform;

	Skybox(std::string path, std::string fileType);
	~Skybox();

	void Render();
};