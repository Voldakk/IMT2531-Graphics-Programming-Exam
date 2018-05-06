#include "Skybox.hpp"

#include "GL/glew.h"

#include "ModelManager.hpp"
#include "Material.hpp"
#include "Components/Transform.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"

namespace EVA
{
	void SkyBoxMaterial::SetMaterialUniforms(Scene* scene) const
	{
		shader->SetUniformMatrix4Fv("view", Application::mainCamera->viewMatrix);
		shader->SetUniformMatrix4Fv("projection", Application::GetPerspectiveMatrix());

		// Cubemap
		glActiveTexture(GL_TEXTURE0);
		shader->SetUniform1I("material.texture_diffuse", 0);

		if (textureDiffuse.id != 0)
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureDiffuse.id);
	}

	Skybox::Skybox(const std::string &folderPath, const std::string &fileType)
	{
		Set(folderPath, fileType);

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

	void Skybox::Set(const std::string& folderPath, const std::string& fileType)
	{
		m_FolderPath = folderPath;
		m_FileType = fileType;

		// Texture
		m_Texture = TextureManager::GetTextureCubemap(folderPath, fileType);

		// Material
		m_Material = std::make_unique<SkyBoxMaterial>();
		m_Material->SetTexture(Texture::Diffuse, m_Texture);
		m_Material->shader = ShaderManager::CreateOrGetShader("skybox", "skybox.vert", "skybox.frag");
	}
}
