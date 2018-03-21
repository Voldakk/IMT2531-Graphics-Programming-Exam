#include "Skybox.hpp"

#include <string>
#include <vector>

#include "GL/glew.h"
#include "SOIL.h"

#include "Mesh.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "TextureManager.hpp"
#include "Transform.hpp"
#include "Primitive.hpp"
#include "Application.hpp"

Skybox::Skybox(const std::string& path, const std::string& fileType)
{
	// Load textures 
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	std::string sideNames[] = { "xpos", "xneg", "ypos", "yneg", "zpos", "zneg" };

	unsigned int sideIds[] = 
	{ 
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 
	};

	for (size_t i = 0; i < 6; i++)
	{
		auto fullPath = path + sideNames[i];
		fullPath.append(fileType);

		int width, height, channels;

		const auto image = SOIL_load_image(fullPath.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

		glTexImage2D(sideIds[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);
	}

	// Material
	material = std::make_unique<Material>();
	material->AddTexture(TextureType::Diffuse, texture);

	// Mesh
	mesh = std::make_unique<Primitive>(PrimitiveType::CubeInverted);

	// Shader
	shader = std::make_unique<SkyboxShader>();

	// Transform
	transform = std::make_unique<Transform>(nullptr);
}

Skybox::~Skybox()
{
	glDeleteTextures(1, &texture);
}

void Skybox::Render() const
{
	transform->SetPosition(Application::camera.position);

	glDepthMask(GL_FALSE);
	shader->SetUniforms(nullptr, transform.get(), mesh.get(), material.get());
	mesh->Draw();
	glDepthMask(GL_TRUE);
}
