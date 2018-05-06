#pragma once

#include "EVA.hpp"

#include "imgui.h"

namespace EVA
{
	class SceneEditor;

	class EditorWindows
	{
	public:

		enum SelectedType { GameObject, Light, Skybox };

	private:

		SceneEditor* m_Editor;

		float m_MenuBarHeight = 0.0f;

		SelectedType m_SelectedType;
		EVA::GameObject* m_SelectedGameObject = nullptr;
		EVA::Light* m_SelectedLight = nullptr;
		EVA::Skybox* m_SelectedSkybox = nullptr;

	public:

		explicit EditorWindows(SceneEditor* editor);

		void SceneHierarchy();

		void Inspector() const;

		void MenuBar();

		void SelectGameObject(EVA::GameObject* gameObject);
		bool IsSelected(EVA::GameObject* gameObject) const;
		EVA::GameObject* SelectedGameObject() const;
		
		void SelectLight(EVA::Light* light);
		bool IsSelected(EVA::Light* light) const;
		EVA::Light* SelectedLight() const;
		
		void SelectSkybox(EVA::Skybox* skybox);
		bool IsSelected(EVA::Skybox* skybox) const;
		EVA::Skybox* SelectedSkybox() const;

	private:

		void DisplayGameObjectsRecursively(EVA::GameObject* gameObject);

		void GameObjectInspector(float width) const;
		void LightInspector(float width) const;
		void SkyboxInspector(float width) const;
	};

}
