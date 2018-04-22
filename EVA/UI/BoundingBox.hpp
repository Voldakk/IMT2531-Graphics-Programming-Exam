#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/extended_min_max.inl"

namespace EVA
{

	struct BoundingBox
	{
		glm::vec2 min;
		glm::vec2 max;

		BoundingBox(const float minX, const float maxX, const float minY, const float maxY)
			: min(minX, minY), max(maxX, maxY) {}

		inline float Width() const
		{
			return max.x - min.x;
		}

		inline float Height() const
		{
			return max.y - min.y;
		}

		inline glm::vec2 Size() const
		{
			return glm::vec2(max.x - min.x, max.y - min.y);
		}

		inline glm::vec2 Right() const
		{
			return glm::vec2(max.x, min.y + Height() / 2);
		}

		inline glm::vec2 Left() const
		{
			return glm::vec2(min.x, min.y + Height() / 2);
		}

		inline glm::vec2 Top() const
		{
			return glm::vec2(min.x + Width() / 2, max.y);
		}

		inline glm::vec2 Bottom() const
		{
			return glm::vec2(min.x + Width() / 2, min.y);
		}

		inline glm::vec2 Center() const
		{
			return glm::vec2(min.x + Width() / 2, min.y + Height() / 2);
		}
	};

}