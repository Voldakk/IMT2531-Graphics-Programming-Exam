#pragma once

#include <string>
#include <memory>
#include "Parsers/Json.hpp"

#include "Material.hpp"

namespace EVA
{

	class Model;
	class Shader;
	class Transform;

	class SkyBoxMaterial : public Material
	{

	public:

		glm::vec4 skyTint = glm::vec4(1.0f);

		SkyBoxMaterial() = default;

		void SetMaterialUniforms(Scene* scene) const override;
	};

	/**
	 * \brief A skybox
	 */
	class Skybox
	{

		std::shared_ptr<Texture> m_Texture;

		std::shared_ptr<Model> m_Model;
		std::unique_ptr<SkyBoxMaterial> m_Material;
		std::unique_ptr<Transform> m_Transform;

		std::string m_FolderPath;
		std::string m_FileType;

	public:

		const std::string& folderPath = m_FolderPath;
		const std::string& fileType = m_FileType;

		Skybox(const std::string &folderPath, const std::string &fileType);
		explicit Skybox(DataObject data);

		void Render() const;

		void Set(const std::string &folderPath, const std::string &fileType);

		void SetTint(glm::vec4 tint) const;

		void SetTint(glm::vec3 tint) const;

		/**
		* \brief Saves Skybox values to the given DataObject
		* \param data The DataObject
		*/
		void Save(DataObject& data) const;

		void Inspector();
	};

}
