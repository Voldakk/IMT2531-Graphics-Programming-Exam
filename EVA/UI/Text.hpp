#pragma once

#include <map>
#include <memory>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "../Shader.hpp"
#include "EVA/OpenGL.hpp"
#include "BoundingBox.hpp"

namespace EVA
{
	const unsigned int FONT_SIZE = 100;

	class Text
	{
		struct Character 
		{
			unsigned int textureId{}; // Id of the glyph texture
			glm::ivec2   size;        // Size of glyph
			glm::ivec2   bearing;     // Offset from baseline to left/top of glyph
			signed long  advance{};   // Offset to advance to next glyph
		};

		static std::map<GLchar, Character> m_Characters;

		static std::shared_ptr<Shader> m_Shader;

		std::unique_ptr<VertexArray> m_Va;
		std::unique_ptr<VertexBuffer> m_Vb;

	public:

		/**
		 * \brief 
		 */
		static void Init();

		Text();

		/**
		 * \brief 
		 * \param text 
		 * \param x 
		 * \param y 
		 * \param scale 
		 * \param color 
		 */
		void Render(std::string text, float x, float y, float scale, glm::vec3 color) const;

		BoundingBox GetSize(std::string text, float scale) const;
	};

}
