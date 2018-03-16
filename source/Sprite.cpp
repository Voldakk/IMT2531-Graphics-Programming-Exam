#include "Sprite.hpp"

#include <vector>

#include "SOIL.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Application.hpp"
#include "Shaderload.hpp"
#include "TextureManager.hpp"

GLuint vao;
GLuint vbo;
GLuint ebo;
GLuint shaderProgram;

GLuint projectionId;
GLuint translationId;
GLuint rotationId;
GLuint scaleId;
GLuint tintId;

Sprite::Sprite()
{
	position = glm::vec2(0, 0);
	scale = glm::vec2(0.1f, 0.1f);
	rotation = 0.0f;

	tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	currentAnimation = "";
	currentSprite = 0;
	currentPlayDir = 1;
	counter = 0;
}

void Sprite::Init()
{
	// Create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * 4, NULL, GL_DYNAMIC_DRAW);

	// Create ebo
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Elements
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * 2, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(elements), elements);

	// Create shader programm
	shaderProgram = ShaderLoad::CreateProgram("../shaders/texture.vert", "../shaders/texture.frag");

	// Create attributes
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	// Get uniform locations
	projectionId = glGetUniformLocation(shaderProgram, "projection");
	translationId = glGetUniformLocation(shaderProgram, "translation");
	rotationId = glGetUniformLocation(shaderProgram, "rotation");
	scaleId = glGetUniformLocation(shaderProgram, "scale");
	tintId = glGetUniformLocation(shaderProgram, "tint");
}

void Sprite::LoadSpriteSheet(std::string path, glm::vec2 numSprites)
{
	// Clear current sequences
	this->numSprites = numSprites;
	animations.clear();
	currentAnimation = "";

	// Load sprite sheet
	texture = TextureManager::GetTexture(path);
}

void Sprite::LoadTintMask(std::string path)
{
	// Load mask
	tintMask = TextureManager::GetTexture(path);
}

void Sprite::Update(float deltaTime)
{
	if (currentAnimation == "")
		return;

	counter += deltaTime;

	if (counter > 1 / framesPerSec)
	{
		counter = 0;
		currentSprite += currentPlayDir;

		Animation a = animations[currentAnimation];

		switch (a.loopMode)
		{
		case Animation::LoopMode::Loop:
			if (currentSprite >= a.numSprites)
				currentSprite = 0;
			break;

		case Animation::LoopMode::Bounce:
			if (currentSprite >= a.numSprites - 1 || currentSprite <= 0)
				currentPlayDir *= -1;
			break;
		case Animation::LoopMode::None:
			if (currentSprite >= a.numSprites)
				currentSprite = a.numSprites - 1;
			break;

		default:
			break;
		}
	}
}

void Sprite::Render()
{
	// Bind
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tintMask);

	// Update vertices and uv coords
	if (currentAnimation != "")
		UpdateVertices(animations[currentAnimation]);

	// Transformation
	glm::mat4 pm = Application::GetProjectionMatrix();

	glm::mat4 tm = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, 0.0f));
	glm::mat4 sm = glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
	glm::mat4 rm = glm::rotate(glm::mat4(1.0), rotation, { 0.0f, 0.0f, 1.0f });

	glUniformMatrix4fv(projectionId, 1, GL_FALSE, &pm[0][0]);
	glUniformMatrix4fv(translationId, 1, GL_FALSE, &tm[0][0]);
	glUniformMatrix4fv(scaleId, 1, GL_FALSE, &sm[0][0]);
	glUniformMatrix4fv(rotationId, 1, GL_FALSE, &rm[0][0]);

	// Tint
	glUniform4fv(tintId, 1, &tint[0]);

	// Draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

	// Unbind
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Sprite::SetFPS(float fps)
{
	if (fps > 0)
		framesPerSec = fps;
	else
		throw(std::runtime_error("Sprite::SetFPS - fps must be greater than 0"));
}

void Sprite::AddAnimation(std::string name, glm::uvec2 start, glm::uvec2 size, int numSprites, Animation::PlayDirection dir, Animation::LoopMode loopMode)
{
	animations[name] = { start, size, numSprites, dir, loopMode };
}

void Sprite::SetAnimation(std::string name)
{
	if (name == currentAnimation)
		return;

	if (animations.find(name) != animations.end())
	{
		currentAnimation = name;
		currentSprite = 0;
		currentPlayDir = 1;

		UpdateVertices(animations[name]);
	}
}

bool Sprite::ContainsPoint(glm::vec2 point)
{
	glm::vec2 max = position + scale;
	glm::vec2 min = position - scale;

	return (point.x < max.x && point.x > min.x && point.y < max.y && point.y > min.y);
}

bool Sprite::Overlap(Sprite & other)
{
	glm::vec2 max = other.position + other.scale;
	glm::vec2 min = other.position - other.scale;

	return ContainsPoint(max) || ContainsPoint(min) || ContainsPoint(glm::vec2(max.x, min.y)) || ContainsPoint(glm::vec2(min.x, max.y));
}

void Sprite::UpdateVertices(Animation & a)
{
	float w = (float)1 / numSprites.x;
	float h = (float)1 / numSprites.y;

	int x, y;

	if (a.dir == Animation::PlayDirection::Horizontal)
	{
		x = currentSprite % a.size.x + a.start.x;
		y = currentSprite / a.size.x + a.start.y;
	}
	else if (a.dir == Animation::PlayDirection::Vertical)
	{
		x = currentSprite / a.size.y + a.start.x;
		y = currentSprite % a.size.y + a.start.y;
	}
	

	GLfloat vertices[] = {
		-1.0,   1.0f, w*x, h*y, // Top-left
		 1.0f,  1.0f, w*(x + 1), h*y, // Top-right
		 1.0f, -1.0f, w*(x + 1), h*(y + 1), // Bottom-right
		-1.0f, -1.0f, w*x, h*(y + 1)  // Bottom-left
	};

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}
