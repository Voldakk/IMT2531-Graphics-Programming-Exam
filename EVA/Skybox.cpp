#include "Skybox.hpp"

#include <vector>

#include "GL/glew.h"

#include "ModelManager.hpp"
#include "Material.hpp"
#include "Components/Transform.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"

namespace EVA
{

	Skybox::Skybox(const std::string &folderPath, const std::string &fileType)
	{
		assert(!folderPath.empty());
		assert(!fileType.empty());

		// Texture
		m_Texture = TextureManager::GetTextureCubemap(folderPath, fileType);

		// Material
		m_Material = std::make_unique<Material>();
		m_Material->SetTexture(Texture::Diffuse, m_Texture);

		// Shader
		m_Material->shader = ShaderManager::CreateOrGetShader("skybox", "skybox.vert", "skybox.frag");

		// Mesh
		m_Model = ModelManager::Primitive(ModelManager::CubeInverted);

		// Transform
		m_Transform = std::make_unique<Transform>();
	}

	Skybox::Skybox(DataObject data) : Skybox(data.GetString("folderPath", ""), data.GetString("fileExtension", ""))
	{

	}

	void Skybox::Render() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
		m_Transform->SetPosition(Application::mainCamera->transform->position);
		m_Material->Activate(nullptr, m_Transform.get());

		glDepthMask(GL_FALSE);
		m_Model->GetMesh(0)->Draw();
		glDepthMask(GL_TRUE);
	}

}