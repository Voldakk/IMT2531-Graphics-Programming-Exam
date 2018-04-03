#pragma once

#include <string>
#include <memory>

namespace EVA
{

	class Mesh;

	class Shader;

	class Material;

	class Transform;

	class Skybox
	{

		unsigned int m_Texture;

		std::shared_ptr<Mesh> m_Mesh;
		std::unique_ptr<Material> m_Material;
		std::unique_ptr<Transform> m_Transform;

	public:

		Skybox(const std::string &folderPath, const std::string &fileType);

		~Skybox();

		void Render() const;
	};

}