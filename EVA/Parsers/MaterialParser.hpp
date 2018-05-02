#pragma once

#include "Json.hpp"
#include "../Material.hpp"
#include "../ShaderManager.hpp"

namespace EVA
{

	class MaterialParser
	{

	public:

		static std::shared_ptr<Material> Get(const std::string& path)
		{
			auto material = std::make_shared<Material>();
			material->shader = EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");

			const auto sd = Json::Open(path);
			auto& d = (*sd);

			// Tint
			if (d.HasMember("tintDiffuse") && Json::IsVec4(d["tintDiffuse"]))
				material->tintDiffuse = Json::GetVec4(d["tintDiffuse"]);

			// Textures
			if (d.HasMember("textureDiffuse") && d["textureDiffuse"].IsString())
				material->SetTexture(Diffuse, d["textureDiffuse"].GetString());

			if (d.HasMember("textureSpecular") && d["textureSpecular"].IsString())
				material->SetTexture(Specular, d["textureSpecular"].GetString());

			if (d.HasMember("textureNormal") && d["textureNormal"].IsString())
				material->SetTexture(Normal, d["textureNormal"].GetString());

			if (d.HasMember("textureEmission") && d["textureEmission"].IsString())
				material->SetTexture(Emission, d["textureEmission"].GetString());

			return material;
		}
	};

}
