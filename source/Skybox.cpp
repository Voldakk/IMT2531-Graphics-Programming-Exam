#include "Skybox.hpp"

#include <string>
#include <vector>

#include "GL/glew.h"

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

	// Shader
	material->shader = std::make_unique<SkyboxShader>();

	// Mesh
	mesh = std::make_unique<Primitive>(PrimitiveType::CubeInverted);

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
	material->shader->SetUniforms(nullptr, transform.get(), mesh.get(), material.get());

	glDepthMask(GL_FALSE);
	mesh->Draw();
	glDepthMask(GL_TRUE);
}
