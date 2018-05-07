#pragma once

#include "Json.hpp"
#include "../Material.hpp"
#include "../ShaderManager.hpp"

namespace EVA
{

	/**
	 * \brief Parser for material files
	 */
	class MaterialParser
	{

	public:

		/**
		 * \brief Loads a mertertil form a material asset file
		 * \param path The path to the file
		 * \return A pointer to the material
		 */
		static std::shared_ptr<Material> Load(const std::string& path)
		{
			auto material = std::make_shared<Material>();

			material->path = path;
			material->shader = EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");

			const auto sd = Json::Open(path);
			auto& d = (*sd);

			// Tint
			if (d.HasMember("tintDiffuse") && Json::IsVec4(d["tintDiffuse"]))
				material->tintDiffuse = Json::GetVec4(d["tintDiffuse"]);

			// Textures
			if (d.HasMember("textureDiffuse") && d["textureDiffuse"].IsString())
				material->SetTexture(Texture::Diffuse, d["textureDiffuse"].GetString());

			if (d.HasMember("textureSpecular") && d["textureSpecular"].IsString())
				material->SetTexture(Texture::Specular, d["textureSpecular"].GetString());

			if (d.HasMember("textureNormal") && d["textureNormal"].IsString())
				material->SetTexture(Texture::Normal, d["textureNormal"].GetString());

			if (d.HasMember("textureEmission") && d["textureEmission"].IsString())
				material->SetTexture(Texture::Emission, d["textureEmission"].GetString());

			return material;
		}

		static void Save(Material* material, const std::string& path)
		{
			Json::Document d;
			d.SetObject();

			auto& a = d.GetAllocator();

			DataObject data(d, &a);

			if (material->tintDiffuse != glm::vec4(1.0f))
				data.SetVec4("tintDiffuse", material->tintDiffuse);

			if (material->textureDiffuse.id != 0)
				data.SetString("textureDiffuse", material->textureDiffuse.path);

			if (material->textureSpecular.id != 0)
				data.SetString("textureSpecular", material->textureSpecular.path);

			if (material->textureNormal.id != 0)
				data.SetString("textureNormal", material->textureNormal.path);

			if (material->textureEmission.id != 0)
				data.SetString("textureEmission", material->textureEmission.path);

			Json::Save(&d, path);
		}
	};

}
