#include "Text.hpp"

#include <iostream>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "../ShaderManager.hpp"
#include "../Application.hpp"

namespace EVA
{

	std::map<GLchar, Text::Character> Text::m_Characters;

	std::shared_ptr<Shader> Text::m_Shader;

	void Text::Init()
	{
		std::cout << "Text::Init \n";
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

		FT_Face face;
		if (FT_New_Face(ft, "./assets/fonts/arial.ttf", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

		FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			m_Characters.insert(std::pair<GLchar, Character>(c, character));
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		m_Shader = EVA::ShaderManager::CreateOrGetShader("text", "text.vert", "text.frag");
	}

	Text::Text()
	{
		// VAO and VBO
		m_Va = std::make_unique<VertexArray>();
		m_Vb = std::make_unique<VertexBuffer>(sizeof(GLfloat) * 6 * 4, GL_DYNAMIC_DRAW);

		VertexBufferLayout layout;
		layout.Push<float>(4); // Position and uv

		m_Va->AddBuffer(*m_Vb, layout);
	}

	void Text::Render(std::string text, GLfloat x, const float y, const float scale, const glm::vec3 color) const
	{
		// Activate corresponding render state	
		m_Shader->Bind();
		m_Shader->SetUniform3Fv("textColor", color);
		m_Shader->SetUniformMatrix4Fv("projection", Application::GetOrthographicMatrix());
		glActiveTexture(GL_TEXTURE0);

		m_Va->Bind();

		for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
		{
			const auto ch = m_Characters[*c];

			const auto xpos = x + ch.bearing.x * scale;
			const auto ypos = y + (ch.bearing.y - ch.size.y) * scale;

			const auto w = ch.size.x * scale;
			const auto h = ch.size.y * scale;

			// Update VBO for each character
			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
			};

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureId);

			// Update content of VBO memory
			m_Vb->BufferData(vertices, sizeof(vertices));

			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
		m_Va->Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	BoundingBox Text::GetSize(std::string text, const float scale) const
	{
		auto x = 0.0f;
		auto miny =  1000000.0f;
		auto maxy = -10000000.0f;

		for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
		{
			const auto ch = m_Characters[*c];

			const auto xpos = x + ch.bearing.x * scale;
			const auto ypos = (ch.bearing.y - ch.size.y) * scale;

			const auto w = ch.size.x * scale;
			const auto h = ch.size.y * scale;

			const auto highY = ypos + h;

			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)

			if (ypos < miny)
				miny = ypos;
			if (highY > maxy)
				maxy = highY;
		}

		return BoundingBox(0.0f, x, miny, maxy);
	}
}
