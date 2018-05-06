#pragma once

#include "EVA.hpp"

#include "imgui.h"

namespace EVA
{
	class SceneEditor;

	class EditorWindows
	{
		SceneEditor* m_Editor;

		

	public:

		explicit EditorWindows(SceneEditor* editor);

		void SceneHierarchy() const;

		void Inspector() const;

	private:

		void DisplayGameObjectsRecursively(GameObject* gameObject) const;
	};

}
