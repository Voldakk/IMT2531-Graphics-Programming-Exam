#include "MeshRenderer.hpp"

#include "Scene.hpp"

MeshRenderer::MeshRenderer()
{

}

void MeshRenderer::Render()
{
	shader->SetUniforms(gameObject->scene, gameObject->transform.get(), mesh.get(), material.get());
	mesh->Draw();
}