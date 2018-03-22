#include "MeshRenderer.hpp"

#include "Scene.hpp"

MeshRenderer::MeshRenderer(GameObject * gameobject) : Component(gameobject)
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
	material->shader->SetUniforms(gameObject->scene, gameObject->transform.get(), mesh.get(), material.get());
	mesh->Draw();
}