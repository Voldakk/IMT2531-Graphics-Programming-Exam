#include "Mesh.hpp"

#include "GL/glew.h"

#include "Scene.hpp"
#include "OBJLoader.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices)
{
	this->vertices = vertices;
	Create();
}

void Mesh::Create()
{
	// Create VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Bind the VAO and VBO
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Buffer vertices
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Bind EBO and buffer face indices
	if (!faceIndices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndices.size() * sizeof(unsigned int), &faceIndices[0], GL_STATIC_DRAW);
	}
	
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
	
	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

void Mesh::Draw() const
{
	// Draw
	glBindVertexArray(vao);

	if(!faceIndices.empty())
		glDrawElements(GL_TRIANGLES, faceIndices.size(), GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

	// Reset
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetIbo(const std::vector<glm::mat4>& models)
{
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

	glBindVertexArray(vao);
	// vertex Attributes
	const GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void Mesh::DrawInstanced(const int count) const
{
	// Draw
	glBindVertexArray(vao);

	if (!faceIndices.empty())
		glDrawElementsInstanced(GL_TRIANGLES, faceIndices.size(), GL_UNSIGNED_INT, nullptr, count);
	else
		glDrawArraysInstanced(GL_TRIANGLES, 0, this->vertices.size(), count);

	// Reset
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

bool Mesh::HasIbo() const
{
	return ibo != 0;
}

std::shared_ptr<Mesh> Mesh::Load(const std::string& path)
{
	auto meshes = OBJLoader::Load(path.c_str());
	if (!meshes.empty())
		return meshes[0];
	else
		return nullptr;
}

std::vector<std::shared_ptr<Mesh>> Mesh::LoadMultiple(const std::string& path)
{
	return  OBJLoader::Load(path.c_str());
}

std::shared_ptr<Mesh> Mesh::Primitive(const PrimitiveType type)
{
	std::string path = "./assets/primitives/";

	switch (type)
	{
	case Circle:
		path += "circle.obj";
		break;
	case Cone:
		path += "cone.obj";
		break;
	case Cube:
		path += "cube.obj";
		break;
	case CubeInverted:
		path += "cube_inverted.obj";
		break;
	case Cylinder:
		path += "cylinder.obj";
		break;
	case Monkey:
		path += "monkey_low.obj";
		break;
	case MonkeyHigh:
		path += "monkey_high.obj";
		break;
	case Icosphere:
		path += "icosphere.obj";
		break;
	case Plane:
		path += "plane.obj";
		break;
	case Sphere:
		path += "sphere.obj";
		break;
	case Torus:
		path += "torus.obj";
		break;
		// Default to cube
	default:
		path += "cube.obj";
		break;
	}

	return Load(path);
}
