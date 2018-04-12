#include "Skybox.hpp"

#include <vector>

#include "GL/glew.h"

#include "Mesh.hpp"
#include "Material.hpp"
#include "Transform.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

namespace EVA
{

	Skybox::Skybox(const std::string &folderPath, const std::string &fileType)
	{
		// Texture
		m_Texture = TextureManager::GetTextureCubemap(folderPath, fileType);

		// Material
		m_Material = std::make_unique<Material>();
		m_Material->SetTexture(TextureType::Diffuse, m_Texture);

		// Shader
		m_Material->shader = std::make_shared<SkyboxShader>();

		// Mesh
		m_Mesh = Mesh::Primitive(PrimitiveType::CubeInverted);

		// Transform
		m_Transform = std::make_unique<Transform>(nullptr);
	}

	Skybox::~Skybox()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void Skybox::Render() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
		m_Transform->SetPosition(Application::mainCamera->transform->position);
		m_Material->Activate(nullptr, m_Transform.get());

		glDepthMask(GL_FALSE);
		m_Mesh->Draw();
		glDepthMask(GL_TRUE);
	}

}