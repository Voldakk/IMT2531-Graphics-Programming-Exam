#pragma once

#include <string>
#include <memory>
#include "Parsers/Json.hpp"

namespace EVA
{

	class Model;
	class Shader;
	class Material;
	class Transform;

	/**
	 * \brief A skybox
	 */
	class Skybox
	{

		unsigned int m_Texture;

		std::shared_ptr<Model> m_Model;
		std::unique_ptr<Material> m_Material;
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
	};

}
