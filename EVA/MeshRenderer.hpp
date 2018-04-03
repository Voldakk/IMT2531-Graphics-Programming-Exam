#pragma once

#include <memory>

#include "Mesh.hpp"	

#include "Material.hpp"	
#include "Component.hpp"

namespace EVA
{

	class MeshRenderer : public Component
	{

		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Material> m_Material;

	public:

		explicit MeshRenderer(GameObject *gameObject);

		void Set(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

		void Render();

		inline std::shared_ptr<Mesh> GetMesh()
		{
			return m_Mesh;
		}

		inline std::shared_ptr<Material> GetMaterial()
		{
			return m_Material;
		}
	};

}