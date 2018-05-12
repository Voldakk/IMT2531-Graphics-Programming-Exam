#include "ModelManager.hpp"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace EVA
{
	std::map<FS::path, std::shared_ptr<Model>> ModelManager::m_Models;

	std::shared_ptr<Model> ModelManager::LoadModel(const FS::path& path)
	{
		// Return the model if it's already loaded
		if (m_Models.count(path))
			return m_Models[path];

		
		std::cout << "ModelManager::LoadModel - " << FileSystem::ToString(path) << "\n";

		// Assimp import
		Assimp::Importer importer;
		const aiScene* scene;
		try
		{
			scene = importer.ReadFile(FileSystem::ToString(path), aiProcess_Triangulate | aiProcess_CalcTangentSpace);
		}
		catch(std::exception& e)
		{
			std::cout << "ModelManager::LoadMesh - Error importing mesh:" << e.what() << std::endl;
			return nullptr;
		}

		// Check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ModelManager::LoadMesh - " << importer.GetErrorString() << std::endl;
			return nullptr;
		}

		// Convert from assimp scene to model
		auto model = std::make_shared<Model>();
		model->path = path;

		ProcessNode(scene->mRootNode, scene, model);

		m_Models[path] = model;
		return model;
	}

	void ModelManager::ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Model>& model)
	{
		// Process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			const auto mesh = scene->mMeshes[node->mMeshes[i]];
			model->AddMesh(ProcessMesh(mesh, scene));
		}

		// Process the child nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model);
		}
	}

	std::shared_ptr<Mesh> ModelManager::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Temp storage
		std::vector<Vertex> vertices;
		std::vector<unsigned int> faceIndices;
		Vertex vertex;

		// Get all the vertex data
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// Positions
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			// Normals
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			// Tangent
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			// Bitangent
			vertex.bitangent.x = mesh->mBitangents[i].x;
			vertex.bitangent.y = mesh->mBitangents[i].y;
			vertex.bitangent.z = mesh->mBitangents[i].z;

			// Texture coordinates
			if (mesh->mTextureCoords[0])
			{
				vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.texCoords = glm::vec2(0.0f, 0.0f);
			}

			// Add the vertex to the mesh
			vertices.push_back(vertex);
		}

		// Read all the face indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const auto face = mesh->mFaces[i];
			
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				faceIndices.push_back(face.mIndices[j]);
			}
		}


		// Create and return the mesh
		return std::make_shared<Mesh>(vertices, faceIndices, mesh->mName.C_Str());;
	}

	std::shared_ptr<Model> ModelManager::Primitive(const PrimitiveType type)
	{
		auto path = PRIMITIVES_PATH;

		switch (type)
		{
		case Circle:
			path += "circle";
			break;
		case Cone:
			path += "cone";
			break;
		case Cube:
			path += "cube";
			break;
		case CubeInverted:
			path += "cube_inverted";
			break;
		case Cylinder:
			path += "cylinder";
			break;
		case Monkey:
			path += "monkey_low";
			break;
		case MonkeyHigh:
			path += "monkey_high";
			break;
		case Icosphere:
			path += "icosphere";
			break;
		case Plane:
			path += "plane";
			break;
		case Sphere:
			path += "sphere";
			break;
		case Torus:
			path += "torus";
			break;
			// Default to cube
		default:
			path += "cube";
			break;
		}

		return LoadModel(path + PRIMITIVES_TYPE);
	}
}