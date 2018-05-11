#pragma once

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class ComponentInspector
{
public:

	static const int STRING_LENGTH = 10000;

	static void Text(const char* text)
	{
		ImGui::Text(text, nullptr);
	}

	static void Int(const char* name, int& value)
	{
		ImGui::InputInt(name, &value);
	}

	static bool EnterInt(const char* name, int& value)
	{
		auto temp = value;
		if( ImGui::InputInt(name, &temp, 1, 5, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			value = temp;
			return true;
		}

		return false;
	}

	static void Bool(const char* name, bool& value)
	{
		ImGui::Checkbox(name, &value);
	}

	static void Float(const char* name, float& value)
	{
		ImGui::InputFloat(name, &value);
	}

	static void Float2(const char* name, glm::vec2& value)
	{
		ImGui::InputFloat2(name, glm::value_ptr(value));
	}

	static void Float3(const char* name, glm::vec3& value)
	{
		ImGui::InputFloat3(name, glm::value_ptr(value));
	}

	static void Float4(const char* name, glm::vec4& value)
	{
		ImGui::InputFloat4(name, glm::value_ptr(value));
	}

	static void String(const char* name, std::string& value)
	{
		const auto string = new char[STRING_LENGTH];
		strcpy(string, value.c_str());

		ImGui::InputText(name, string, STRING_LENGTH);

		value = string;

		delete[] string;
	}

	static bool EnterString(const char* name, std::string& value)
	{
		const auto string = new char[STRING_LENGTH];
		strcpy(string, value.c_str());

		if (ImGui::InputText(name, string, STRING_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			value = string;
			delete[] string;
			return true;
		}

		delete[] string;
		return false;
	}

	static bool DragDropTargetString(const char* name, std::string& value, const char* payloadType)
	{
		const auto string = new char[STRING_LENGTH];
		strcpy(string, value.c_str());

		ImGui::BeginGroup();
		if (ImGui::InputText(name, string, STRING_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			value = string;
			delete[] string;
			ImGui::EndGroup();
			return true;
		}
		delete[] string;
		if (ImGui::BeginDragDropTarget())
		{
			const auto payload = ImGui::AcceptDragDropPayload(payloadType);
			if (payload)
			{
				const char* path = (char*)payload->Data;
				value = path;

				ImGui::EndDragDropTarget();
				ImGui::EndGroup();
				return true;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::EndGroup();

		return false;
	}

	static bool ColorPicker(const char* name, glm::vec3& value)
	{
		return ImGui::ColorPicker3(name, glm::value_ptr(value));
	}
};
