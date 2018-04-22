#pragma once

#include <memory>

#include "EVA/OpenGL.hpp"
#include "../Shader.hpp"

namespace EVA
{
	struct BoundingBox;

	class SpriteRenderer
	{
		static std::shared_ptr<Shader> m_Shader;
		static std::unique_ptr<VertexArray> m_Va;
		static std::unique_ptr<VertexBuffer> m_Vb;

		static std::vector<SpriteRenderer*> m_SpriteRenderers;

	public:

		unsigned int texture;
		glm::vec3 tint;

		explicit SpriteRenderer();

		void Render(const BoundingBox& bb) const;
	};
}