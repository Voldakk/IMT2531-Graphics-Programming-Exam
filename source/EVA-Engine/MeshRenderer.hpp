#pragma once

#include <memory>

#include "Mesh.hpp"	

#include "Material.hpp"	
#include "Component.hpp"

class MeshRenderer : public Component
{
public:

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	explicit MeshRenderer(GameObject * gameObject);

	void Set(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	void Render();
};