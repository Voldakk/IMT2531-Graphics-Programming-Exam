#pragma once
#include <vector>
#include <memory>

#include "Mesh.hpp"

namespace EVA
{

	class Model
	{
		std::vector<std::shared_ptr<Mesh>> m_Meshes;

	public:

		void AddMesh(const std::shared_ptr<Mesh>& mesh);

		std::shared_ptr<Mesh> GetMesh(const unsigned int index) const;
		std::shared_ptr<Mesh> GetMesh(const std::string& name) const;

		inline unsigned int MeshCount() const { return  m_Meshes.size(); }
	};

}