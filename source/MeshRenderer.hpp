#pragma once

#include <memory>

#include "Mesh.hpp"	
#include "Shader.hpp"
#include "Material.hpp"	
#include "GameObject.hpp"

class MeshRenderer : public Component
{
public:

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	std::shared_ptr<Shader> shader;

	MeshRenderer();

	virtual void Render() override;
};

