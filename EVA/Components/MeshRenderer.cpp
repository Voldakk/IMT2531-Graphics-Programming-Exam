#include "MeshRenderer.hpp"

#include "../Scene.hpp"

namespace EVA
{
	MeshRenderer::~MeshRenderer()
	{
		scene->RemoveMeshRenderer(this);
	}

	void MeshRenderer::Set(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
	{
		this->m_Mesh = mesh;
		this->m_Material = material;
		scene->RegisterMeshRenderer(this);
	}

	void MeshRenderer::Render() const
	{
		m_Material->Activate(scene.Get(), transform.Get());
		m_Mesh->Draw();
	}
}