#pragma once

#include "EVA.hpp"

#include "imgui.h"

#include "../FileSystem.hpp"

namespace EVA
{
	class SceneEditor;

	class EditorWindows
	{
	public:

		enum SelectedType { GameObject, Light, Skybox, Path };

	private:

		SceneEditor* m_Editor;

		float m_MenuBarHeight = 0.0f;

		SelectedType m_SelectedType;
		EVA::GameObject* m_SelectedGameObject = nullptr;
		EVA::Light* m_SelectedLight = nullptr;
		EVA::Skybox* m_SelectedSkybox = nullptr;
		FS::path m_SelectedPath;

		float m_HierarchyWidth = 0.0f;
		float m_InspectorWidth = 0.0f;

		// Assets browser
		FS::path m_SelectedAssetFolder = "./assets";

		enum NewAssetType { Folder, Material, Shader };
		NewAssetType m_NewAssetType;
		FS::path m_NewPath = "";

		FS::path m_RenamePath = "";
		FS::path m_DeletePath = "";

		const unsigned int m_NameBufferSize = 10000;
		char* m_NameBuffer = "";

	public:

		explicit EditorWindows(SceneEditor* editor);

		~EditorWindows();

		void SceneHierarchy();

		void Inspector();

		void MenuBar();

		void AssetBrowser();

		void SelectGameObject(EVA::GameObject* gameObject);
		bool IsSelected(EVA::GameObject* gameObject) const;
		EVA::GameObject* SelectedGameObject() const;
		
		void SelectLight(EVA::Light* light);
		bool IsSelected(EVA::Light* light) const;
		EVA::Light* SelectedLight() const;
		
		void SelectSkybox(EVA::Skybox* skybox);
		bool IsSelected(EVA::Skybox* skybox) const;
		EVA::Skybox* SelectedSkybox() const;

		void SelectPath(const FS::path& path);
		bool IsSelected(const FS::path& path) const;
		FS::path SelectedPath() const;

	private:
		
		void DisplayFoldersRecursively(const FS::path& path);

		void DisplayGameObjectsRecursively(EVA::GameObject* gameObject);
		
		void GameObjectInspector(float width) const;

		void LightInspector() const;

		void SkyboxInspector() const;

		void MaterialInspector() const;

		void ShaderInspector() const;

		void NewAssetPopup();

		void RenameAssetPopup();

		void DeleteAssetPopup();

	};

}
