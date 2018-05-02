#pragma once

#include <cstdio>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

namespace EVA
{

	class Json
	{

	public:

		static std::shared_ptr<rapidjson::Document> Open(const std::string& path)
		{
			const auto fp = fopen(path.c_str(), "rb"); // non-Windows use "r"
			char readBuffer[65536];

			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

			auto d = std::make_shared<rapidjson::Document>();
			d->ParseStream(is);

			return d;
		}

		static bool IsVec4(rapidjson::Value& value)
		{
			if (value.IsArray())
			{
				const auto a = value.GetArray();
				if (a.Size() == 4 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble() && a[3].IsDouble())
					return true;
			}

			return false;
		}

		static glm::vec4 GetVec4(rapidjson::Value& value)
		{
			const auto a = value.GetArray();
			return { a[0].GetDouble() , a[1].GetDouble() , a[2].GetDouble() , a[3].GetDouble() };
		}
	};

}