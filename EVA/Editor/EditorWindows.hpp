#pragma once

#include "EVA.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.inl"

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
				if (gameObject->GetName() == "EVA::EditorCamera")
					continue;

				if (gameObject->transform->parent.Get() == nullptr)
					DisplayGameObjectsRecursively(gameObject.get(), selected);
			}

			ImGui::End();
		}

		static void Inspector(GameObject* gameObject)
		{
			const auto screenSize = Application::GetWindowSize();
			ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)screenSize.y }, { (float)screenSize.x, (float)screenSize.y });
			ImGui::SetNextWindowPos({ (float)screenSize.x, 0.0f }, 0, {1.0f, 0.0f});

			ImGui::Begin("Inspector");
			const auto windowSize = ImGui::GetWindowSize();

			if (gameObject != nullptr)
			{
				// Name
				ImGui::Text("Name");
				const auto gameObjectName = new char[10000];

				strcpy_s(gameObjectName, sizeof(gameObject->GetName()), gameObject->GetName().c_str());

				ImGui::PushItemWidth(windowSize.x);
				ImGui::InputText("##Line",gameObjectName, 10000);

				gameObject->SetName(gameObjectName);

				ImGui::Spacing();

				// Transform
				ImGui::Text("Position");
				auto position = gameObject->transform->localPosition;
				ImGui::InputFloat3("transformPosition", glm::value_ptr(position), "%.7g");
				
				gameObject->transform->SetPosition(position);

				ImGui::Text("Rotation");
				auto rotation = gameObject->transform->localRotation;
				ImGui::InputFloat3("transformRotation", glm::value_ptr(rotation), "%.7g");
				gameObject->transform->SetOrientation(rotation);
				

				ImGui::Text("Scale");
				auto scale = gameObject->transform->localScale;
				ImGui::InputFloat3("transformScale", glm::value_ptr(scale), "%.7g");
				gameObject->transform->SetScale(scale);

				// Other components
				ImGui::Text("Components");
				for (const auto& component : gameObject->GetComponents())
				{
					if (ImGui::CollapsingHeader(component->GetTypeId().c_str()))
					{
						ImGui::Text("This is a component");

						ImGui::Spacing();
					}
				}
			}

			ImGui::End();
		}
	};

}
