#pragma once
#include <memory>

#include "Json.hpp"
#include "../Shader.hpp"

namespace EVA
{
	

	class ShaderParser
	{
	public:

		static std::shared_ptr<ShaderPaths> LoadShader(const FS::path& path)
		{
			auto shaderPaths = std::make_shared<ShaderPaths>();

			shaderPaths->shader = path;

			const auto sd = Json::Open(path);

			if (sd == nullptr)
			{
				std::cout << "ShaderParser::LoadShader - Invalid shader file: " << FileSystem::ToString(path) << std::endl;
				return nullptr;
			}

			auto& d = (*sd);

			if (!d.IsObject())
			{
				std::cout << "ShaderParser::LoadShader - Invalid shader file: " << FileSystem::ToString(path) << std::endl;
				return nullptr;
			}

			DataObject data(d);

			shaderPaths->vertex = data.GetPath("vertex", "");
			shaderPaths->fragment = data.GetPath("fragment", "");
			shaderPaths->geometry = data.GetPath("geometry", "");

			return shaderPaths;
		}
	};

}
