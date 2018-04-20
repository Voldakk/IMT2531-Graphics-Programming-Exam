#pragma once

#include <map>
#include <memory>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "Shader.hpp"
#include "EVA/OpenGL.hpp"

namespace EVA
{

	class Text
	{
		struct Character 
		{
			unsigned int textureId{}; // Id of the glyph texture
			glm::ivec2   size;        // Size of glyph
			glm::ivec2   bearing;     // Offset from baseline to left/top of glyph
			unsigned int advance{};   // Offset to advance to next glyph
		};

		static std::map<GLchar, Character> m_Characters;

		static std::shared_ptr<Shader> m_Shader;
		static std::unique_ptr<VertexArray> m_Va;
		static std::unique_ptr<VertexBuffer> m_Vb;

		static glm::mat4 m_Projection;

	public:

		/**
		 * \brief 
		 */
		static void Init();

		/**
		 * \brief 
		 * \param text 
		 * \param x 
		 * \param y 
		 * \param scale 
		 * \param color 
		 */
		static void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	};

}