#include "MeshRenderer.hpp"

#include "../Scene.hpp"
#include "../ModelManager.hpp"
#include "../Parsers/MaterialParser.hpp"
#include "../MaterialManager.hpp"

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

		if (mesh != nullptr)
		{
			m_ModelPath = m_Mesh->path;
			m_MeshIndex = m_Mesh->index;
		}
		
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
			material = MaterialParser::Load(materialPath);
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
		const auto modelPath = data.GetString("mesh", "");
		const auto meshIndex = data.GetInt("meshIndex", 0);		

		if(!modelPath.empty())
		{
			const auto model = ModelManager::LoadModel(modelPath);
			if(model->MeshCount() > meshIndex)
				mesh = model->GetMesh(meshIndex);
		}

		Set(mesh, material);
	}

	void MeshRenderer::Save(DataObject& data)
	{
		if (material != nullptr)
			data.SetString("material", FileSystem::ToString(material->path));

		if(mesh != nullptr)
		{
			data.SetString("mesh", FileSystem::ToString(mesh->path));
			data.SetInt("meshIndex", mesh->index);
		}
	}

	void MeshRenderer::Inspector()
	{
		auto materialPath = material != nullptr ? FileSystem::ToString(material->path) : "";
		if (ComponentInspector::DragDropTargetString("Material", materialPath, "file"))
		{
			if (!materialPath.empty())
			{
				Set(m_Mesh, MaterialManager::LoadMaterial(materialPath));
			}
		}

		auto modelPath = FileSystem::ToString(m_ModelPath);
		if (ComponentInspector::DragDropTargetString("Model", modelPath, "file"))
		{
			if (!modelPath.empty())
			{
				SetMesh(modelPath, m_MeshIndex);
			}
		}

		int meshIndex = m_MeshIndex;
		if(ComponentInspector::EnterInt("Mesh index", meshIndex))
		{
			if (meshIndex >= 0)
			{
				SetMesh(m_ModelPath, meshIndex);
			}
		}
	}

	void MeshRenderer::SetMesh(const FS::path& modelPath, const unsigned meshIndex)
	{
		m_MeshIndex = meshIndex;
		m_ModelPath = modelPath;

		if (!modelPath.empty())
		{
			const auto model = ModelManager::LoadModel(modelPath);
			if (model->MeshCount() > meshIndex)
			{
				const auto mesh	= model->GetMesh(meshIndex);
				Set(mesh, m_Material);
			}
		}
	}
}
