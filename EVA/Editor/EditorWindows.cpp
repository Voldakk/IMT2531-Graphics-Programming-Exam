#include "EditorWindows.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "../SceneEditor.hpp"

namespace EVA
{

	EditorWindows::EditorWindows(SceneEditor* editor)
	{
		m_Editor = editor;
	}

	void EditorWindows::SceneHierarchy() const
	{
		const auto windowSize = Application::GetWindowSize();
		ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)windowSize.y }, { (float)windowSize.x, (float)windowSize.y });
		ImGui::SetNextWindowPos({ 0.0f, 0.0f });

		ImGui::Begin("Scene Hierarchy");

		for (auto& gameObject : m_Editor->GetGameObjects())
		{
			if (gameObject->GetName() == "EVA::EditorCamera")
				continue;

			if (gameObject->transform->parent.Get() == nullptr)
				DisplayGameObjectsRecursively(gameObject.get());
		}

		ImGui::End();
	}

	void EditorWindows::Inspector() const
	{
		const auto screenSize = Application::GetWindowSize();
		ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)screenSize.y }, { (float)screenSize.x, (float)screenSize.y });
		ImGui::SetNextWindowPos({ (float)screenSize.x, 0.0f }, 0, { 1.0f, 0.0f });

		ImGui::Begin("Inspector");
		const auto windowSize = ImGui::GetWindowSize();

		auto gameObject = m_Editor->GetSelected();

		if (gameObject != nullptr)
		{
			// Name
			ImGui::Text("Name");
			const auto gameObjectName = new char[10000];

			strcpy_s(gameObjectName, sizeof(gameObject->GetName()), gameObject->GetName().c_str());

			ImGui::PushItemWidth(windowSize.x);
			ImGui::InputText("##Line", gameObjectName, 10000);

			gameObject->SetName(gameObjectName);

			ImGui::Spacing();

			// Transform
			ImGui::Text("Position");
			auto position = gameObject->transform->localPosition;
			ImGui::InputFloat3("InspectorTransformPosition", glm::value_ptr(position), "%.7g");

			gameObject->transform->SetPosition(position);

			ImGui::Text("Rotation");
			auto rotation = gameObject->transform->localRotation;
			ImGui::InputFloat3("InspectorTransformRotation", glm::value_ptr(rotation), "%.7g");
			gameObject->transform->SetOrientation(rotation);


			ImGui::Text("Scale");
			auto scale = gameObject->transform->localScale;
			ImGui::InputFloat3("InspectorTransformScale", glm::value_ptr(scale), "%.7g");
			gameObject->transform->SetScale(scale);

			// Other components
			ImGui::Text("Components");
			const auto components = gameObject->GetComponents();
			for (unsigned int i = 0; i < components.size(); ++i)
			{
				auto component = components[i];

				if (ImGui::CollapsingHeader((std::to_string(i) + " " + component->GetTypeId()).c_str()))
				{
					ImGui::Text("This is a component");

					ImGui::Spacing();
				}
			}

			// Add component
			if(ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("InspectorAddComponentSelect");
			}

			if (ImGui::BeginPopup("InspectorAddComponentSelect"))
			{
				auto ids = ComponentMap::GetComponentIds();
				ImGui::Separator();

				for (const auto& id : ids)
				{
					if (ImGui::Selectable(id.c_str()))
					{
						const auto component = ComponentMap::CreateComponent(id);
						if (component != nullptr)
						{
							component->SetScene(gameObject->scene.Get());
							gameObject->AttachComponent(component);

							component->Awake();
							component->Start();
						}
					}
				}
				ImGui::EndPopup();
			}

		}

		ImGui::End();
	}

	void EditorWindows::DisplayGameObjectsRecursively(GameObject* gameObject) const
	{
		const auto nodeFlags =
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_OpenOnArrow |
			(gameObject == m_Editor->GetSelected() ? ImGuiTreeNodeFlags_Selected : 0) |
			(gameObject->transform->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0);

		if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), nodeFlags))
		{
			if (ImGui::IsItemClicked())
				m_Editor->SetSelected(gameObject);

			for (auto& child : gameObject->transform->GetChildren())
			{
				DisplayGameObjectsRecursively(child->gameObject.Get());
			}

			ImGui::TreePop();
		}
	}

}
