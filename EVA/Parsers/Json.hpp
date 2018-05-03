#pragma once

#include <cstdio>
#include <memory>

#include "glm/glm.hpp"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

namespace EVA
{
	

	class Json
	{

	public:

		typedef rapidjson::Value Value;
		typedef rapidjson::Document Document;
		typedef rapidjson::GenericValue<rapidjson::UTF8<>> Generic;

		static std::shared_ptr<Document> Open(const std::string& path)
		{
			const auto fp = fopen(path.c_str(), "rb");
			char readBuffer[65536];

			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

			auto d = std::make_shared<rapidjson::Document>();
			d->ParseStream(is);

			return d;
		}

		static bool IsVec4(Value& jsonValue)
		{
			if (jsonValue.IsArray())
			{
				const auto a = jsonValue.GetArray();
				if (a.Size() == 4 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble() && a[3].IsDouble())
					return true;
			}

			return false;
		}

		static glm::vec4 GetVec4(Value& jsonValue)
		{
			const auto a = jsonValue.GetArray();
			return { a[0].GetDouble() , a[1].GetDouble() , a[2].GetDouble() , a[3].GetDouble() };
		}

		static float GetFloat(const Value& json, const char* key, const float defaultValue)
		{
			if(json.HasMember(key) && json[key].IsNumber())
				return (float)json[key].GetDouble();

			return defaultValue;
		}
	};

	class DataObject
	{
		Json::Generic& m_Json;

	public:

		explicit DataObject(Json::Generic& json) : m_Json(json)
		{

		}

		int GetInt(const char* key, const int defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsInt())
				return m_Json[key].GetInt();

			return defaultValue;
		}

		bool GetBool(const char* key, const bool defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsBool())
				return m_Json[key].GetBool();

			return defaultValue;
		}

		float GetFloat(const char* key, const float defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsNumber())
				return (float)m_Json[key].GetDouble();

			return defaultValue;
		}

		glm::vec2 GetVec2(const char* key, const glm::vec2 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 2 && a[0].IsDouble() && a[1].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble() };
			}

			return defaultValue;
		}

		glm::vec3 GetVec3(const char* key, const glm::vec3 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 3 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble() };
			}

			return defaultValue;
		}

		glm::vec4 GetVec4(const char* key, const glm::vec4 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 4 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble() && a[3].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble(), a[3].GetDouble() };
			}

			return defaultValue;
		}

		std::string GetString(const char* key, const std::string& defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsString())
				return m_Json[key].GetString();

			return defaultValue;
		}

	};

}