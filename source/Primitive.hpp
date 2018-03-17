#pragma once

#include "Mesh.hpp"

enum PrimitiveType { Cube };

class Primitive :
	public Mesh
{
public:
	Primitive(PrimitiveType type);
	~Primitive();
};

