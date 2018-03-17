#include "Mesh.hpp"

#include <string>

#include "GL/glew.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "OBJLoader.hpp"
#include "Shaderload.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

Mesh::Mesh(const char * path)
{
	OBJLoader::Load(path, vertices);

	shader = ShaderLoad::CreateProgram("../shaders/standard.vert", "../shaders/standard.frag");
	viewID = glGetUniformLocation(shader, "view");
	projectionID = glGetUniformLocation(shader, "projection");
	modelID = glGetUniformLocation(shader, "model");
}

Mesh::~Mesh()
{

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

void Mesh::Render()
{
	glm::vec3 lightSourcePosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightColor = glm::vec3(0.8f, 1.0f, 0.8f);
	float attenuationA = 1.0f;
	float attenuationB = 0.2f;
	float attenuationC = 0.0f;

	GLint normalMatrixID = glGetUniformLocation(shader, "normalMatrix");
	
	
	GLint attenuationAID = glGetUniformLocation(shader, "attenuationA");
	GLint attenuationBID = glGetUniformLocation(shader, "attenuationB");
	GLint attenuationCID = glGetUniformLocation(shader, "attenuationC");
	GLint lightID = glGetUniformLocation(shader, "lightColor");
	GLint lightSourcePositionID = glGetUniformLocation(shader, "lightSourcePosition");
	GLint CamPosID = glGetUniformLocation(shader, "CamPos");

	glUniform1f(attenuationAID, attenuationA);
	glUniform1f(attenuationBID, attenuationB);
	glUniform1f(attenuationCID, attenuationC);
	glUniform3fv(lightID, 1, value_ptr(lightColor));
	glUniform3fv(lightSourcePositionID, 1, value_ptr(lightSourcePosition));
	glUniform3fv(CamPosID, 1, value_ptr(glm::vec3(0.0f)));


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

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view*model)));
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));

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
