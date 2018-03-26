#include "MeshRenderer.hpp"

#include "Scene.hpp"

MeshRenderer::MeshRenderer(GameObject * gameObject) : Component(gameObject)
{
	
}

void MeshRenderer::Set(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{
	this->mesh = mesh;
	this->material = material;
	gameObject->scene->RegisterMeshRenderer(this);
}

void MeshRenderer::Render()
{
	material->Activate(gameObject->scene, gameObject->transform.get());
	mesh->Draw();
}