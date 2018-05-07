#pragma once

#define RAPIDJSON_HAS_STDSTRING 1

#include <cstdio>
#include <memory>

#include "glm/glm.hpp"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

namespace EVA
{
	

	class Json
	{

	public:

		typedef rapidjson::Value Value;
		typedef rapidjson::Document Document;
		typedef rapidjson::GenericValue<rapidjson::UTF8<>> Generic;
		typedef rapidjson::Document::AllocatorType Allocator;
		typedef rapidjson::GenericStringRef<char> StringRef;

		static std::shared_ptr<Document> Open(const std::string& path)
		{
			const auto fp = fopen(path.c_str(), "rb");
			char readBuffer[65536];

			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

			auto d = std::make_shared<rapidjson::Document>();
			d->ParseStream(is);

			return d;
		}

		static bool Save(const Document* d, const std::string& path)
		{
			const auto fp = fopen(path.c_str(), "wb");
			char writeBuffer[65536];
			rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
			rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
			const auto r = d->Accept(writer);
			fclose(fp);

			return r;
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
		Json::Allocator* m_Allocator;
	public:

		explicit DataObject(Json::Generic& json) : m_Json(json), m_Allocator(nullptr)
		{}

		explicit DataObject(Json::Generic& json, Json::Allocator* allocator) : m_Json(json), m_Allocator(allocator)
		{}

		int GetInt(const char* key, const int defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsInt())
				return m_Json[key].GetInt();

			return defaultValue;
		}

		void SetInt(const Json::StringRef& key, const int value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

		bool GetBool(const char* key, const bool defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsBool())
				return m_Json[key].GetBool();

			return defaultValue;
		}

		void SetBool(const Json::StringRef& key, const bool value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

		float GetFloat(const char* key, const float defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsNumber())
				return (float)m_Json[key].GetDouble();

			return defaultValue;
		}

		void SetFloat(const Json::StringRef& key, const float value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
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

		void SetVec2(const Json::StringRef& key, const glm::vec2 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
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

		void SetVec3(const Json::StringRef& key, const glm::vec3 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);
			v.PushBack(value.z, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
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

		void SetVec4(const Json::StringRef& key, const glm::vec4 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);
			v.PushBack(value.z, *m_Allocator);
			v.PushBack(value.w, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
		}

		std::string GetString(const char* key, const std::string& defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsString())
				return m_Json[key].GetString();

			return defaultValue;
		}

		void SetString(const Json::StringRef& key, const std::string& value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

	};

}