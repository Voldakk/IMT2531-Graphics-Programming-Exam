#pragma once

#include "EVA.hpp"

#include "imgui.h"

namespace EVA
{
	class SceneEditor;

	class EditorWindows
	{
		SceneEditor* m_Editor;

		float m_MenuBarHeight = 0.0f;

	public:

		explicit EditorWindows(SceneEditor* editor);

		void SceneHierarchy() const;

		void Inspector() const;

		void MenuBar();

	private:

		void DisplayGameObjectsRecursively(GameObject* gameObject) const;
	};

}
