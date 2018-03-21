#pragma once

#include <memory>

#include "Mesh.hpp"	
#include "Shader.hpp"
#include "Material.hpp"	
#include "Component.hpp"

class MeshRenderer : public Component
{
public:

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Material> material;

	explicit MeshRenderer(GameObject * gameobject);

	virtual void Render() override;
};