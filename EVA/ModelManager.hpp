#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "Model.hpp"
#include "Mesh.hpp"

namespace EVA
{
	enum PrimitiveType
	{
		Circle, Cone, Cube, CubeInverted, Cylinder, Icosphere, Monkey, MonkeyHigh, Plane, Sphere, Torus
	};

	class ModelManager
	{

		static std::map<std::string, std::shared_ptr<Model>> m_Models;

	public:

		/**
		 * \brief Load a model from the specified path
		 * \param path The path to the model
		 * \return A pointer to the model
		 */
		static std::shared_ptr<Model> LoadModel(const std::string& path);
		
		/**
		 * \brief Loads a primitive model
		 * \param type The type of primitive
		 * \return A pointer to the model
		 */
		static std::shared_ptr<Model> Primitive(PrimitiveType type);

	private:

		static void ProcessNode(aiNode *node, const aiScene *scene, const std::shared_ptr<Model>& model);
		static std::shared_ptr<Mesh>  ProcessMesh(aiMesh *mesh, const aiScene *scene);
	};
}
