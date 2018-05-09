#pragma once

#include <memory>

#include "../Mesh.hpp"	

#include "../Material.hpp"	
#include "../Component.hpp"
#include "../ComponentMap.hpp"

namespace EVA
{

	class MeshRenderer : public Component
	{
		REGISTER_COMPONENT_HPP(MeshRenderer)

		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Material> m_Material;

		FS::path m_ModelPath;
		unsigned int m_MeshIndex;

	public:

		// Public read only fields
		const std::shared_ptr<Mesh>& mesh = m_Mesh;
		const std::shared_ptr<Material>& material = m_Material;

		~MeshRenderer() override;

		void Start() override;

		void Set(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);

		void Render() const;

		/**
		* \brief Loads Component values from the given DataObject
		* \param data The DataObject
		*/
		void Load(DataObject data) override;

		/**
		* \brief Saves Component values to the given DataObject
		* \param data The DataObject
		*/
		void Save(DataObject& data) override;

		/**
		* \brief Draws the component inspector
		*/
		void Inspector() override;

	private:

		void SetMesh(const FS::path& modelPath, unsigned int meshIndex);
		
	};
}
