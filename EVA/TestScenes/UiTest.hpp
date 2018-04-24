#pragma once

#include "EVA.hpp"
#include "EVA/UI.hpp"
#include "EVA/Input.hpp"

namespace EVA_TEST
{
	class UiTest : public EVA::Scene
	{

	public:

		UiTest::UiTest()
		{
			// Cursor mode
			EVA::Input::SetCursorMode(EVA::Input::Normal);

			// Centered layout with 3 items
			const auto label = CreateUiElement<EVA::Button>("UI TEST", 0.005f);

			auto layout = CreateUiElement<EVA::Layout>();
			layout->SetMargin(0.1f);

			layout->AddChild(label);
			layout->AddChild(CreateUiElement<EVA::Button>("Item 1", 0.002f));
			layout->AddChild(CreateUiElement<EVA::Button>("Item 2", 0.002f));

			// Corners
			auto topLeft = CreateUiElement<EVA::Button>("Top Left");
			topLeft->SetAnchor(-1.0f, 1.0f);
			topLeft->SetPivot(-1.0f, 1.0f);
			topLeft->SetOffset(0.05f, -0.05f);

			auto bottomLeft = CreateUiElement<EVA::Button>("Bottom Left");
			bottomLeft->SetAnchor(-1.0f, -1.0f);
			bottomLeft->SetPivot(-1.0f, -1.0f);
			bottomLeft->SetOffset(0.05f, 0.05f);

			auto topRight = CreateUiElement<EVA::Button>("Top Right");
			topRight->SetAnchor(1.0f, 1.0f);
			topRight->SetPivot(1.0f, 1.0f);
			topRight->SetOffset(-0.05f, -0.05f);

			auto bottomRight = CreateUiElement<EVA::Button>("Bottom Right");
			bottomRight->SetAnchor(1.0f, -1.0f);
			bottomRight->SetPivot(1.0f, -1.0f);
			bottomRight->SetOffset(-0.05f, 0.05f);
		}
	};
}
