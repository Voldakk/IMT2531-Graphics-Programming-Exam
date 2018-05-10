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
				return nullptr;

			auto& d = (*sd);

			DataObject data(d);

			shaderPaths->vertex = data.GetPath("vertex", "");
			shaderPaths->fragment = data.GetPath("fragment", "");
			shaderPaths->geometry = data.GetPath("geometry", "");

			return shaderPaths;
		}
	};

}
