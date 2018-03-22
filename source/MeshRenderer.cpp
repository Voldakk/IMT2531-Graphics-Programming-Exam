#include "MeshRenderer.hpp"

#include "Scene.hpp"

MeshRenderer::MeshRenderer(GameObject * gameobject) : Component(gameobject)
{
	
}

void MeshRenderer::Render()
{
	material->shader->SetUniforms(gameObject->scene, gameObject->transform.get(), mesh.get(), material.get());
	mesh->Draw();
}