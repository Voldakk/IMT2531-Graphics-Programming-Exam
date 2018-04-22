#include "SpriteRenderer.hpp"

#include "../ShaderManager.hpp"
#include "../Transform.hpp"
#include "../Application.hpp"
#include "BoundingBox.hpp"

namespace EVA
{

	std::shared_ptr<Shader> SpriteRenderer::m_Shader;
	std::unique_ptr<VertexArray> SpriteRenderer::m_Va;
	std::unique_ptr<VertexBuffer> SpriteRenderer::m_Vb;

	SpriteRenderer::SpriteRenderer() 
		: texture(0), tint(glm::vec3(1.0f))
	{
		if (m_Shader == nullptr)
		{
			m_Shader = ShaderManager::CreateOrGetShader("sprite", "sprite.vert", "sprite.frag");

			float vertices[6][4] = 
			{
				{ 1.0f, 0.0f, 1.0, 0.0 },
				{ 1.0f, 1.0f, 1.0, 1.0 },
				{ 0.0f, 1.0f, 0.0, 1.0 },

				{ 0.0f, 1.0f, 0.0, 1.0 },
				{ 0.0f, 0.0f, 0.0, 0.0 },
				{ 1.0f, 0.0f, 1.0, 0.0 }
			};

			m_Va = std::make_unique<VertexArray>();
			m_Vb = std::make_unique<VertexBuffer>(vertices, sizeof(GLfloat) * 6 * 4, GL_STATIC_DRAW);

			VertexBufferLayout layout;
			layout.Push<float>(2); // Position
			layout.Push<float>(2); // Uv

			m_Va->AddBuffer(*m_Vb, layout);
		}
	}

	void SpriteRenderer::Render(const BoundingBox& bb) const
	{
		// Shader 
		m_Shader->Bind();
		m_Shader->SetUniform3Fv("tint", tint);
		m_Shader->SetUniform2Fv("position", bb.min);
		m_Shader->SetUniform2Fv("size", bb.Size());
		m_Shader->SetUniformMatrix4Fv("projection", Application::GetOrthographicMatrix());

		// Texture
		m_Shader->SetUniform1I("texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// VAO
		m_Va->Bind();

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
