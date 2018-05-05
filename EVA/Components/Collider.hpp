#pragma once

#include "EVA.hpp"

namespace EVA
{
	struct Bounds
	{
		glm::vec3 center = glm::vec3(0.0f);
		glm::vec3 extents = glm::vec3(1.0f);

		Bounds() = default;

		Bounds(const glm::vec3 center, const glm::vec3 extents)
		{
			this->center = center;
			this->extents = extents;
		}
	};

	class Collider : public Component
	{

	public:
		
		Bounds bounds;

		~Collider();

		void Awake() override;

		virtual Bounds GetBounds() = 0;

	};

}
