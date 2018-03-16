#include "Text.hpp"

#include "Shaderload.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

Text::Text(std::string text, glm::vec2 pos, bool center, float size, glm::vec4 tint)
{
    this->tint = tint;
    this->pos = pos;
    this->center = center;
    this->size = size;
    
    // Create texture and shader program
    texture = TextureManager::GetTexture("../assets/font_5.png");
    shaderProgram = ShaderLoad::CreateProgram("../shaders/texture.vert", "../shaders/texture.frag");

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
	float maxX = text.length() * size;
    
	// For each character
	for (size_t i = 0; i < text.length(); i++)
	{
		// Find the position of each corner
        glm::vec2 vertex_up_left    = glm::vec2( pos.x+i*size     , pos.y      );
        glm::vec2 vertex_up_right   = glm::vec2( pos.x+i*size+size, pos.y      );
        glm::vec2 vertex_down_right = glm::vec2( pos.x+i*size+size, pos.y-size );
        glm::vec2 vertex_down_left  = glm::vec2( pos.x+i*size     , pos.y-size );

		// Offset the positions if the text is centered
        if(center)
        {
            vertex_up_left += glm::vec2( -maxX/2, size/2);
            vertex_up_right += glm::vec2( -maxX/2, size/2);
            vertex_down_right += glm::vec2( -maxX/2, size/2);
            vertex_down_left += glm::vec2( -maxX/2, size/2);
        }

        vertices.push_back(vertex_up_left   );
        vertices.push_back(vertex_down_left );
        vertices.push_back(vertex_up_right  );

        vertices.push_back(vertex_down_right);
        vertices.push_back(vertex_up_right);
        vertices.push_back(vertex_down_left);

		// Get the character's index in the texture
        int character = text[i];
        if(character < 'a' || character > 'z')
            character--;

        double w = 1/16.0;
        double h = 1/8.0;
        int x = character % 16;
        int y = character / 16;

		// The UV coordinates
        glm::vec2 uv_up_left    = glm::vec2( x*w,       y*h);
        glm::vec2 uv_up_right   = glm::vec2( (x+1)*w,   y*h);
        glm::vec2 uv_down_right = glm::vec2( (x+1)*w,   (y+0.75f)*h);
        glm::vec2 uv_down_left  = glm::vec2( x*w,       (y+0.75f)*h);

        UVs.push_back(uv_up_left   );
        UVs.push_back(uv_down_left );
        UVs.push_back(uv_up_right  );

        UVs.push_back(uv_down_right);
        UVs.push_back(uv_up_right);
        UVs.push_back(uv_down_left);
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
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_attribute);

	// Color attribute
	GLint color_attribute = glGetAttribLocation(shaderProgram, "texcoord");
	glVertexAttribPointer(color_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(GLfloat) * 2 * vertices.size()));
	glEnableVertexAttribArray(color_attribute);

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
	glm::mat4 pm = Application::GetProjectionMatrix();
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, &pm[0][0]);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}