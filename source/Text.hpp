#pragma once
#include <vector>
#include <string>

#include "GL/glew.h"
#include "glm/glm/glm.hpp"

// A text object to be displayed on screen
class Text
{
    public:
		/// <summary>Constructor. Creates VAO, VBO and shader program</summary>
		/// <param name="text">The displayed text</param>  
		/// <param name="position">The position of the text</param>  
		/// <param name="center">Whether the text is centered around the position</param>  
		/// <param name="size">The size of one leter</param>  
		/// <param name="tint">The color of the text</param>  
        Text(std::string text, glm::vec2 position = glm::vec2(0.0f, 0.0f), bool center = false, float size = 0.1, glm::vec4 tint = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		/// <summary>Changes the displayed text
		/// <para>Generates the vertex positions and UV coordinates for the new text</para> 
		/// <para>Buffers the new data to the VBO</para> 
		/// </summary>
		/// <param name="text">The displayed text</param>
        void SetText(std::string text);

		/// <summary>Renders the text to the screen</summary>
		void Render();
    
    private:
        std::vector<glm::vec2> vertices;
        std::vector<glm::vec2> UVs;
        glm::vec4 tint;

        GLuint vao;
        GLuint vbo;
        GLuint shaderProgram;
        GLuint texture;
		GLuint tintId;

		GLuint projectionId;

        glm::vec2 pos;
        bool center; 
        float size;
};