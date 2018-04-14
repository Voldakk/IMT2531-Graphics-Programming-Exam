#include "Model.hpp"

namespace EVA
{

	void Model::AddMesh(const std::shared_ptr<Mesh>& mesh)
	{
		m_Meshes.push_back(mesh);
	}

	std::shared_ptr<Mesh> Model::GetMesh(const unsigned int index) const
	{
		return m_Meshes[index];
	}

	std::shared_ptr<Mesh> Model::GetMesh(const std::string& name) const
	{
		for (const auto& mesh : m_Meshes)
		{
			if (mesh->name == name)
				return mesh;
		}

		return nullptr;
	}
}
