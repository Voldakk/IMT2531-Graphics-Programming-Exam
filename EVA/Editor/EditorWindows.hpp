#pragma once

#include "EVA.hpp"
#include "imgui.h"

namespace EVA
{

	class EditorWindows
	{

		static void DisplayGameObjectsRecursively(GameObject* gameObject, GameObject** selected)
		{
			const auto nodeFlags = 
				ImGuiTreeNodeFlags_OpenOnDoubleClick | 
				ImGuiTreeNodeFlags_OpenOnArrow | 
				(gameObject == *selected ? ImGuiTreeNodeFlags_Selected : 0) | 
				(gameObject->transform->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0);

			if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), nodeFlags))
			{
				if (ImGui::IsItemClicked())
					*selected = gameObject;

				for (auto& child : gameObject->transform->GetChildren())
				{
					DisplayGameObjectsRecursively(child->gameObject.Get(), selected);
				}

				ImGui::TreePop();
			}
		}

	public:
		 
		static void SceneHierarchy(Scene* scene, GameObject** selected)
		{
			const auto windowSize = Application::GetWindowSize();
			ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)windowSize.y }, { (float)windowSize.x, (float)windowSize.y });
			ImGui::SetNextWindowPos({ 0.0f, 0.0f });

			ImGui::Begin("Scene Hierarchy");

			for (auto& gameObject : scene->GetGameObjects())
			{
				if(gameObject->GetName() == "EVA::EditorCamera")
					continue;

				if(gameObject->transform->parent.Get() == nullptr)
					DisplayGameObjectsRecursively(gameObject.get(), selected);
			}

			ImGui::End();
		}
	};

}
