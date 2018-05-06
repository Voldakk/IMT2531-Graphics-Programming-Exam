#include "EditorWindows.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "SceneEditor.hpp"

namespace EVA
{

	EditorWindows::EditorWindows(SceneEditor* editor)
	{
		m_Editor = editor;
	}

	void EditorWindows::SceneHierarchy()
	{
		const auto windowSize = Application::GetWindowSize();
		ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)windowSize.y - m_MenuBarHeight }, { (float)windowSize.x, (float)windowSize.y - m_MenuBarHeight });
		ImGui::SetNextWindowPos({ 0.0f, m_MenuBarHeight });

		const auto flags = ImGuiWindowFlags_ResizeFromAnySide;
		ImGui::Begin("Scene Hierarchy", nullptr, flags);

		// Skybox
		if (m_Editor->skybox != nullptr)
		{
			const auto nodeFlags =
				ImGuiTreeNodeFlags_OpenOnDoubleClick |
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_Leaf |
				(IsSelected(m_Editor->skybox.get()) ? ImGuiTreeNodeFlags_Selected : 0);

			if (ImGui::TreeNodeEx("Skybox", nodeFlags))
			{
				// Select on click
				if (ImGui::IsItemClicked())
					SelectSkybox(m_Editor->skybox.get());

				// Context menu
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
						if (IsSelected(m_Editor->skybox.get()))
							SelectSkybox(nullptr);
						m_Editor->skybox = nullptr;
					}

					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}

		// Lights
		if (ImGui::TreeNodeEx("Lights"))
		{
			auto lights = m_Editor->GetLights();

			for (unsigned int i = 0; i < lights.size(); ++i)
			{
				const auto nodeFlags =
				ImGuiTreeNodeFlags_OpenOnDoubleClick |
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_Leaf |
				(IsSelected(lights[i].get()) ? ImGuiTreeNodeFlags_Selected : 0);

				if (ImGui::TreeNodeEx(("Light #" + std::to_string(i)).c_str(), nodeFlags))
				{
					// Select on click
					if (ImGui::IsItemClicked())
						SelectLight(lights[i].get());

					// Context menu
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete"))
						{
							if (IsSelected(lights[i].get()))
								SelectLight(nullptr);
							m_Editor->DestroyLight(lights[i].get());
						}

						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		// Game objects
		auto gameObjects = m_Editor->GetGameObjects();
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetName() == "EVA::EditorCamera")
				continue;

			if (gameObject->transform->parent.Get() == nullptr)
				DisplayGameObjectsRecursively(gameObject.get());
		}

		// Context menu
		if (ImGui::BeginPopupContextWindow(nullptr, 1, false))
		{
			if (ImGui::MenuItem("Add game object"))
			{
				auto newGameObject = m_Editor->CreateGameObject();
				newGameObject->SetName("New GameObject");
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void EditorWindows::Inspector() const
	{
		const auto screenSize = Application::GetWindowSize();
		ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)screenSize.y - m_MenuBarHeight }, { (float)screenSize.x, (float)screenSize.y - m_MenuBarHeight });
		ImGui::SetNextWindowPos({ (float)screenSize.x, m_MenuBarHeight }, 0, { 1.0f, 0.0f });

		const auto flags = ImGuiWindowFlags_ResizeFromAnySide;
		ImGui::Begin("Inspector", nullptr, flags);

		const auto windowSize = ImGui::GetWindowSize();

		switch (m_SelectedType)
		{
		case GameObject:
			GameObjectInspector(windowSize.x);
			break;
		case Light:
			LightInspector(windowSize.x);
			break;
		case Skybox:
			SkyboxInspector(windowSize.x);
			break;
		default: 
			break;
		}

		ImGui::End();
	}

	void EditorWindows::MenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			m_MenuBarHeight = ImGui::GetWindowSize().y;

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("GameObject"))
				{
					auto newGameObject = m_Editor->CreateGameObject();
					newGameObject->SetName("New GameObject");
				}

				if (ImGui::BeginMenu("Component"))
				{
					auto ids = ComponentMap::GetComponentIds();
					

					for (const auto& id : ids)
					{
						if (ImGui::MenuItem(id.c_str()) && SelectedGameObject() != nullptr)
						{
							const auto component = ComponentMap::CreateComponent(id);
							if (component != nullptr)
							{
								component->SetScene(SelectedGameObject()->scene.Get());
								SelectedGameObject()->AttachComponent(component);

								component->Awake();
								component->Start();
							}
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Light"))
				{
					if (ImGui::MenuItem("Directional"))
					{
						m_Editor->CreateLight(Light::Directional);
					}
					if (ImGui::MenuItem("Point"))
					{
						m_Editor->CreateLight(Light::Point);
					}

					ImGui::EndMenu();
				}

				if(m_Editor->skybox == nullptr)
				{
					if (ImGui::MenuItem("Skybox"))
					{
						m_Editor->skybox = std::make_unique<EVA::Skybox>("", "");
					}
				}
				

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void EditorWindows::SelectGameObject(EVA::GameObject* gameObject)
	{
		m_SelectedType = GameObject;
		m_SelectedGameObject = gameObject;
	}

	bool EditorWindows::IsSelected(EVA::GameObject* gameObject) const
	{
		return m_SelectedType == GameObject && m_SelectedGameObject == gameObject;
	}

	EVA::GameObject* EditorWindows::SelectedGameObject() const
	{
		return m_SelectedType == GameObject ? m_SelectedGameObject : nullptr;
	}
	
	void EditorWindows::SelectLight(EVA::Light* light)
	{
		m_SelectedType = Light;
		m_SelectedLight = light;
	}

	bool EditorWindows::IsSelected(EVA::Light* light) const
	{
		return m_SelectedType == Light && m_SelectedLight == light;
	}

	EVA::Light* EditorWindows::SelectedLight() const
	{
		return m_SelectedType == Light ? m_SelectedLight : nullptr;
	}

	void EditorWindows::SelectSkybox(EVA::Skybox* skybox)
	{
		m_SelectedType = Skybox;
		m_SelectedSkybox = skybox;
	}

	bool EditorWindows::IsSelected(EVA::Skybox* skybox) const
	{
		return m_SelectedType == Skybox && m_SelectedSkybox == skybox;
	}

	EVA::Skybox* EditorWindows::SelectedSkybox() const
	{
		return m_SelectedType == Skybox ? m_SelectedSkybox : nullptr;
	}


	void EditorWindows::DisplayGameObjectsRecursively(EVA::GameObject* gameObject)
	{
		const auto nodeFlags =
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_OpenOnArrow |
			(IsSelected(gameObject) ? ImGuiTreeNodeFlags_Selected : 0) |
			(gameObject->transform->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0);

		if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), nodeFlags))
		{
			// Select on click
			if (ImGui::IsItemClicked())
				SelectGameObject(gameObject);

			// Context menu
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Add game object"))
				{
					auto newGameObject = m_Editor->CreateGameObject();
					newGameObject->SetName("New GameObject");
					newGameObject->SetParent(gameObject);
				}
				if (ImGui::MenuItem("Delete"))
				{
					gameObject->Destroy();
					if (IsSelected(gameObject))
						SelectGameObject(nullptr);

					ImGui::EndPopup();
					ImGui::TreePop();
					return;
				}

				ImGui::EndPopup();
			}

			// Display children
			for (auto& child : gameObject->transform->GetChildren())
			{
				DisplayGameObjectsRecursively(child->gameObject.Get());
			}

			ImGui::TreePop();
		}
	}

	void EditorWindows::GameObjectInspector(const float width) const
	{
		auto gameObject = SelectedGameObject();

		if (gameObject == nullptr)
			return;

		ImGui::PushItemWidth(width);

		// Name
		ImGui::Text("Name");
		const auto gameObjectName = new char[10000];

		strcpy_s(gameObjectName, sizeof(gameObject->GetName()), gameObject->GetName().c_str());

		if (ImGui::InputText("##Line", gameObjectName, 10000, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			gameObject->SetName(gameObjectName);
		}

		delete[] gameObjectName;

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
			bool keep = true;
			if (ImGui::CollapsingHeader((std::to_string(i) + " " + component->GetTypeId()).c_str(), &keep))
			{

				ImGui::Text("This is a component");

				ImGui::Spacing();
			}
			if (!keep)
			{
				gameObject->RemoveComponent(component.get());
			}
		}

		// Add component
		if (ImGui::Button("Add Component"))
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

		ImGui::PopItemWidth();
	}

	void EditorWindows::LightInspector(const float width) const
	{
		auto light = SelectedLight();
		if(light == nullptr)
			return;

		ImGui::Text(light->GetType() == Light::Directional ? "Directional light" : "Point light");

		ImGui::ColorEdit3("Color", glm::value_ptr(light->color));
		ImGui::InputFloat("Ambient coefficient", &light->ambientCoefficient);

		if(light->GetType() == Light::Directional)
		{
			ImGui::InputFloat("Shadow distance", &light->directionalShadowDistance);
			ImGui::InputFloat("Near plane", &light->directionalNearPlane);
			ImGui::InputFloat("Far plane", &light->directionalFarPlane);
		}
		else
		{
			auto position = light->position;
			ImGui::InputFloat3("Position", glm::value_ptr(position));
			light->SetPosition(position);

			ImGui::InputFloat("Attenuation", &light->attenuation);
			ImGui::InputFloat("Near plane", &light->pointNearPlane);
			ImGui::InputFloat("Far plane", &light->pointFarPlane);
		}
	}

	void EditorWindows::SkyboxInspector(const float width) const
	{
		auto skybox = SelectedSkybox();

		if(skybox == nullptr)
			return;

		ImGui::Text("Skybox");

		const auto folderPath = new char[10000];
		strcpy_s(folderPath, sizeof(skybox->folderPath), skybox->folderPath.c_str());

		const auto fileType = new char[10000];
		strcpy_s(fileType, sizeof(skybox->fileType), skybox->fileType.c_str());

		if (ImGui::InputText("Folder path", folderPath, 10000, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			skybox->Set(folderPath, fileType);
		}
		if (ImGui::InputText("File type", fileType, 10000, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			skybox->Set(folderPath, fileType);
		}

		delete[] folderPath;
		delete[] fileType;
	}
}
