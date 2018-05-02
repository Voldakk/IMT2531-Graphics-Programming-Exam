#pragma once

#include "EVA.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace EVA
{

	class FollowTarget : public Component, public IUpdateComponent
	{

	public:

		Transform * target;
		glm::vec3 offset = glm::vec3(0.0f);
		float scalar = 1.0f;
		

		FollowTarget()
		{
			target = nullptr;
		}

		void Update(const float deltaTime) override
		{
			if(target == nullptr)
				return;

			transform->SetOrientation(glm::toQuat(glm::lookAtLH(transform->position, target->position, YAXIS)));


			const auto targetPos = target->position + target->LocalToWorld(offset);
			transform->SetPosition(glm::mix(transform->position, targetPos, deltaTime * scalar));
		}
	};
}