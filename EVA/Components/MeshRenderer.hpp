#pragma once

#include <memory>

#include "../Mesh.hpp"	

#include "../Material.hpp"	
#include "../Component.hpp"

namespace EVA
{

	class MeshRenderer : public Component
	{

		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Material> m_Material;

	public:

		// Public read only fields
		const std::shared_ptr<Mesh>& mesh = m_Mesh;
		const std::shared_ptr<Material>& material = m_Material;

		MeshRenderer() = default;

		~MeshRenderer() override;

		void Set(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);

		void Render() const;
		
	};
}