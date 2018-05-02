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

	class Skybox
	{

		unsigned int m_Texture;

		std::shared_ptr<Model> m_Model;
		std::unique_ptr<Material> m_Material;
		std::unique_ptr<Transform> m_Transform;

	public:

		Skybox(const std::string &folderPath, const std::string &fileType);
		explicit Skybox(DataObject data);

		void Render() const;
	};

}
