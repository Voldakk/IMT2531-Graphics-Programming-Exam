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

		shader->SetUniform4Fv("skyTint", skyTint);

		// Cubemap
		glActiveTexture(GL_TEXTURE0);
		shader->SetUniform1I("material.texture_diffuse", 0);

		if (textureDiffuse != nullptr)
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureDiffuse->id);
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
		if(m_Texture == nullptr)
			return;

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
		m_Texture = TextureManager::LoadTextureCubemap(folderPath, fileType);
		if(m_Texture != nullptr)
			m_Texture->type = Texture::Diffuse;

		// Material
		m_Material = std::make_unique<SkyBoxMaterial>();
		m_Material->SetTexture(m_Texture);
		m_Material->shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "skybox.shader");
	}

	void Skybox::SetTint(const glm::vec4 tint) const
	{
		if (m_Material != nullptr)
			m_Material->skyTint = tint;
	}

	void Skybox::SetTint(const glm::vec3 tint) const
	{
		if (m_Material != nullptr)
			m_Material->skyTint = glm::vec4(tint, 1.0f);
	}

	void Skybox::Save(DataObject& data) const
	{
		if (!m_FolderPath.empty())
			data.SetString("folderPath", m_FolderPath);

		if (!m_FolderPath.empty())
			data.SetString("fileExtension", m_FileType);
	}

	void Skybox::Inspector()
	{
		if (ComponentInspector::DragDropTargetString("Folder path", m_FolderPath, "folder") || 
			ComponentInspector::EnterString("File type", m_FileType))
		{
			Set(m_FolderPath, m_FileType);
		}

		if(m_Material != nullptr)
			ComponentInspector::ColorPicker("Tint", m_Material->skyTint);
	}
}
