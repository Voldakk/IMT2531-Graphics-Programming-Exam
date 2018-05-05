#pragma once

#include "FreeLook.hpp"

#include "EVA/Input.hpp"
#include "EVA/Physics.hpp"

namespace EVA
{
	
	class SceneCamera : public FreeLook
	{
		GameObject* m_Selected = nullptr;

	public:
		void Update(float deltaTime) override;
	};

	inline void SceneCamera::Update(const float deltaTime)
	{
		if (Input::MouseButton(Input::Button::MouseRight))
		{
			Input::SetCursorMode(Input::Disabled);
			FreeLook::Update(deltaTime);
		}
		else
		{
			Input::SetCursorMode(Input::Normal);

			if (Input::MouseButtonDown(Input::MouseLeft))
			{
				const auto mousePos = Input::MousePosition();
				const auto ray = Physics::ScreenPosToWorldRay(mousePos, Application::mainCamera);

				RaycastHit hit;
				if (Physics::Raycast(ray, hit, scene.Get()))
				{
					std::cout << "Name: " << hit.hitCollider->gameObject->GetName() << ", Dist: " << hit.distance 
					<< ", Point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ") \n";

					m_Selected = hit.hitCollider->gameObject.Get();
				}

			}
		}
	}
}
