#include "OBJLoader.hpp"

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "Mesh.hpp"


bool OBJLoader::Load(const char * path, std::vector<Vertex> & vertices)
{
	std::vector<unsigned int> finVertIdx, finUvIdx, finNormIdx;

	std::vector<glm::vec3> tmp_vert;
	std::vector<glm::vec2> tmp_uv;
	std::vector<glm::vec3> tmp_norms;

	FILE * file = fopen(path, "r");
	if (file == nullptr)
	{
		printf("Drama!");
		return false;
	}

	while (1)
	{
		char symb[130];
		int res = fscanf(file, "%s", symb);
		if (res == EOF) {
			break;
		}

		if (strcmp(symb, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmp_vert.push_back(vertex);
		}
		else
			if (strcmp(symb, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				tmp_uv.push_back(uv);
			}
			else
				if (strcmp(symb, "vn") == 0) {
					glm::vec3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					tmp_norms.push_back(normal);
				}
				else
					if (strcmp(symb, "f") == 0) {
						unsigned int vertIdx[3], uvIdx[3], nnIdx[3];
						int suc = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
							&vertIdx[0], &uvIdx[0], &nnIdx[0],
							&vertIdx[1], &uvIdx[1], &nnIdx[1],
							&vertIdx[2], &uvIdx[2], &nnIdx[2]);
						if (suc != 9) {
							printf("Wrong format for face specification.");
							fclose(file);
							return false;
						}
						finVertIdx.push_back(vertIdx[0]);
						finVertIdx.push_back(vertIdx[1]);
						finVertIdx.push_back(vertIdx[2]);

						finUvIdx.push_back(uvIdx[0]);
						finUvIdx.push_back(uvIdx[1]);
						finUvIdx.push_back(uvIdx[2]);

						finNormIdx.push_back(nnIdx[0]);
						finNormIdx.push_back(nnIdx[1]);
						finNormIdx.push_back(nnIdx[2]);

					}
					else {
						//discard
						char discardBuffer[1000];
						fgets(discardBuffer, 1000, file);
					}
	}

	vertices.clear();

	for (unsigned int i = 0; i < finVertIdx.size(); i++) 
	{

		unsigned int vertIdx = finVertIdx[i];
		unsigned int uvIdx = finUvIdx[i];
		unsigned int nnIdx = finNormIdx[i];

		glm::vec3 vertex = tmp_vert[vertIdx - 1];
		glm::vec2 uv = tmp_uv[uvIdx - 1];
		glm::vec3 normals = tmp_norms[nnIdx - 1];

		
		vertices.push_back({ vertex, normals, uv });
	}

	fclose(file);
}
