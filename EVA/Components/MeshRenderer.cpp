#include "MeshRenderer.hpp"

#include "../Scene.hpp"
#include "../ModelManager.hpp"
#include "../Parsers/MaterialParser.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(MeshRenderer, "EVA::MeshRenderer")

	MeshRenderer::~MeshRenderer()
	{
		scene->RemoveMeshRenderer(this);
	}

	void MeshRenderer::Start()
	{
		
	}

	void MeshRenderer::Set(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
	{
		if (m_Mesh != nullptr || m_Material != nullptr)
			scene->RemoveMeshRenderer(this);

		m_Mesh = mesh;
		m_Material = material;
		
		if (m_Mesh == nullptr || m_Material == nullptr)
			return;

		scene->RegisterMeshRenderer(this);
	}

	void MeshRenderer::Render() const
	{
		m_Material->Activate(scene.Get(), transform.Get());
		m_Mesh->Draw();
	}

	void MeshRenderer::Load(const DataObject data)
	{
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;

		// Material
		const auto materialPath = data.GetString("material", "");
		if (!materialPath.empty())
		{
			material = MaterialParser::Get(materialPath);
		}

		// Mesh primitive
		const auto meshPrimitive = data.GetString("meshPrimitive", "");
		if (!meshPrimitive.empty())
		{
			const auto model = ModelManager::LoadModel(ModelManager::PRIMITIVES_PATH + meshPrimitive + ModelManager::PRIMITIVES_TYPE);
			if (model->MeshCount() >= 1)
				mesh = model->GetMesh(0);
		}

		// Mesh
		const auto meshPath = data.GetString("mesh", "");
		const auto meshIndex = data.GetInt("meshIndex", 0);		

		if(!meshPath.empty())
		{
			const auto model = ModelManager::LoadModel(meshPath);
			if(model->MeshCount() > meshIndex)
				mesh = model->GetMesh(meshIndex);
		}

		Set(mesh, material);
	}
}
