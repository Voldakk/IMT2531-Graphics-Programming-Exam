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

Skybox::Skybox(const std::string& folderPath, const std::string& fileType)
{
	// Texture
	texture = TextureManager::GetTextureCubemap(folderPath, fileType);

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
	transform->SetPosition(Application::mainCamera->gameObject->transform->position);

	glDepthMask(GL_FALSE);
	shader->SetUniforms(nullptr, transform.get(), mesh.get(), material.get());
	mesh->Draw();
	glDepthMask(GL_TRUE);
}
