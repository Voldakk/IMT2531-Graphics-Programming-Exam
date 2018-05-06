#pragma once

#include "EVA.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.inl"

namespace EVA
{
	class SceneEditor;

	class EditorWindows
	{
		SceneEditor* m_Editor;

		void DisplayGameObjectsRecursively(GameObject* gameObject);

	public:

		explicit EditorWindows(SceneEditor* editor);

		void SceneHierarchy();

		void Inspector();
	};

}
