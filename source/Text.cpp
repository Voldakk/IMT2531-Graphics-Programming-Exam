#include "Text.hpp"

#include "Shaderload.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

Text::Text(const std::string& text, const glm::vec2 pos, const bool center, const float size, const glm::vec4 tint)
{
    this->tint = tint;
    this->pos = pos;
    this->center = center;
    this->size = size;
    
    // Create texture and shader program
    texture = TextureManager::GetTexture("./assets/font_5.png");
    shaderProgram = ShaderLoad::CreateProgram("texture.vert", "texture.frag");

    // Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vector Buffer Object that will store the vertices on video memory
	glGenBuffers(1, &vbo);

	// Allocate space for vertex positions and colors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

    // Tint
    tintId = glGetUniformLocation(shaderProgram, "tint");

    SetText(text);
}
void Text::SetText(std::string text)
{
    vertices.clear(),
    UVs.clear();
    
	// The width of the text
	const auto maxX = text.length() * size;
    
	// For each character
	for (size_t i = 0; i < text.length(); i++)
	{
		// Find the position of each corner
		auto vertexUpLeft    = glm::vec2( pos.x+i*size     , pos.y      );
		auto vertexUpRight   = glm::vec2( pos.x+i*size+size, pos.y      );
		auto vertexDownRight = glm::vec2( pos.x+i*size+size, pos.y-size );
		auto vertexDownLeft  = glm::vec2( pos.x+i*size     , pos.y-size );

		// Offset the positions if the text is centered
        if(center)
        {
            vertexUpLeft += glm::vec2( -maxX/2, size/2);
            vertexUpRight += glm::vec2( -maxX/2, size/2);
            vertexDownRight += glm::vec2( -maxX/2, size/2);
            vertexDownLeft += glm::vec2( -maxX/2, size/2);
        }

        vertices.push_back(vertexUpLeft   );
        vertices.push_back(vertexDownLeft );
        vertices.push_back(vertexUpRight  );

        vertices.push_back(vertexDownRight);
        vertices.push_back(vertexUpRight);
        vertices.push_back(vertexDownLeft);

		// Get the character's index in the texture
        int character = text[i];
        if(character < 'a' || character > 'z')
            character--;

		const auto w = 1/16.0;
		const auto h = 1/8.0;
		const auto x = character % 16;
		const auto y = character / 16;

		// The UV coordinates
		const auto uvUpLeft    = glm::vec2( x*w,       y*h);
		const auto uvUpRight   = glm::vec2( (x+1)*w,   y*h);
		const auto uvDownRight = glm::vec2( (x+1)*w,   (y+0.75f)*h);
		const auto uvDownLeft  = glm::vec2( x*w,       (y+0.75f)*h);

        UVs.push_back(uvUpLeft   );
        UVs.push_back(uvDownLeft );
        UVs.push_back(uvUpRight  );

        UVs.push_back(uvDownRight);
        UVs.push_back(uvUpRight);
        UVs.push_back(uvDownLeft);
    }

    // Bind
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * (vertices.size() + UVs.size()), NULL, GL_DYNAMIC_DRAW);

	// Transfer the vertex positions:
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 2 * vertices.size(), vertices.data());

	// Transfer the vertex uvs:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * vertices.size(), sizeof(GLfloat) * 2 * UVs.size(), UVs.data());

    // Get the location of the attributes that enters in the vertex shader
	const auto positionAttribute = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	// Color attribute
	const auto colorAttribute = glGetAttribLocation(shaderProgram, "texcoord");
	glVertexAttribPointer(colorAttribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(GLfloat) * 2 * vertices.size()));
	glEnableVertexAttribArray(colorAttribute);

	// Get uniform locations
	projectionId = glGetUniformLocation(shaderProgram, "projection");
}

void Text::Render()
{
	// Bind
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
	
	// Set tint
    glUniform4fv(tintId, 1, &tint[0]);

	// Projection
	glm::mat4 pm = Application::GetOrthographicMatrix();
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, &pm[0][0]);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}