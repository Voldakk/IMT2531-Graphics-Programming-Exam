#include "Mesh.hpp"

#include <string>

#include "GL/glew.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Scene.hpp"
#include "OBJLoader.hpp"
#include "Shaderload.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

Mesh::Mesh()
{
	LoadShader();
}

Mesh::Mesh(const char * path)
{
	LoadShader();
	LoadMesh(path);
}

void Mesh::Create()
{
	// Create VAO, VBO and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the VAO and VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Buffer vertices
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Bind EBO and buffer face indices
	if (faceIndices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndices.size() * sizeof(unsigned int), &faceIndices[0], GL_STATIC_DRAW);
	}

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void Mesh::Render(Scene * scene)
{
	// Lights
	glUniform1f(materialShininessID, materialShininess);
	glUniform3fv(materialSpecularColorID, 1, value_ptr(materialSpecularColor));

	glUniform3fv(lightPositionID, 1, value_ptr(scene->light.position));
	glUniform3fv(lightIntensitiesID, 1, value_ptr(scene->light.color));

	glUniform1f(lightAttenuationID, scene->light.attenuation);
	glUniform1f(lightAmbientCoefficientID, scene->light.ambientCoefficient);

	// Camera
	glUniform3fv(cameraPositionID, 1, value_ptr(Application::camera.position));

	// Textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// Activate the texture
		glActiveTexture(GL_TEXTURE0 + i);

		// Find the name based on the texture type
		std::string name = "";

		switch (textures[i].type)
		{
		case TextureType::Diffuse:
			name = "texture_diffuse" + std::to_string(diffuseNr++);
			break;
		case TextureType::Specular:
			name = "texture_specular" + std::to_string(specularNr++);
			break;
		case TextureType::Normal:
			name = "texture_normal" + std::to_string(normalNr++);
			break;
		case TextureType::Height:
			name = "texture_height" + std::to_string(heightNr++);
			break;
		default:
			break;
		}

		// Set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader, name.c_str()), i);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// Matrices
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = Application::camera.GetViewMatrix();
	glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = Application::GetProjectionMatrix(false);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw
	glBindVertexArray(VAO);

	if(faceIndices.size() > 0 )
		glDrawElements(GL_TRIANGLES, faceIndices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

	// Reset
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::AddTexture(const TextureType type, const char * path)
{
	Texture t = { 0,type, path };
	t.id = TextureManager::GetTexture(t.path);
	textures.push_back(t);
}

void Mesh::LoadShader()
{
	shader = ShaderLoad::CreateProgram("../shaders/standard.vert", "../shaders/standard.frag");

	viewID = glGetUniformLocation(shader, "view");
	projectionID = glGetUniformLocation(shader, "projection");
	modelID = glGetUniformLocation(shader, "model");

	materialShininessID = glGetUniformLocation(shader, "materialShininess");
	materialSpecularColorID = glGetUniformLocation(shader, "materialSpecularColor");

	lightPositionID = glGetUniformLocation(shader, "light.position");
	lightIntensitiesID = glGetUniformLocation(shader, "light.intensities");

	lightAttenuationID = glGetUniformLocation(shader, "light.attenuation");
	lightAmbientCoefficientID = glGetUniformLocation(shader, "light.ambientCoefficient");

	cameraPositionID = glGetUniformLocation(shader, "cameraPosition");
}

void Mesh::LoadMesh(const char * path)
{
	OBJLoader::Load(path, vertices);
	Create();
}
