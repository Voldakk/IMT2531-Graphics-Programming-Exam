#pragma once

#include "imgui.h"

class ComponentInspector
{
public:

	static const int STRING_LENGTH = 10000;

	static void Text(const char* text)
	{
		ImGui::Text(text);
	}

	static void Int(const char* name, int& value)
	{
		ImGui::InputInt(name, &value);
	}

	static void Bool(const char* name, bool& value)
	{
		ImGui::Checkbox(name, &value);
	}

	static void Float(const char* name, float& value)
	{
		ImGui::InputFloat(name, &value);
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
};