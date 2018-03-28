#include "MeshRenderer.hpp"

#include "Scene.hpp"

namespace EVA
{

	MeshRenderer::MeshRenderer(GameObject *gameObject) : Component(gameObject)
	{

	}

	void MeshRenderer::Set(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
	{
		this->m_Mesh = mesh;
		this->m_Material = material;
		m_GameObject->GetScene()->RegisterMeshRenderer(this);
	}

	void MeshRenderer::Render()
	{
		m_Material->Activate(m_GameObject->GetScene(), m_GameObject->GetTransform().get());
		m_Mesh->Draw();
	}

}