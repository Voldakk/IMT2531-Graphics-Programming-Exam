#pragma once

#include "Mesh.hpp"

enum PrimitiveType { Circle, Cone, Cube, Cylinder, Icosphere, Monkey, MonkeyHigh, Plane, Sphere, Torus };

class Primitive : public Mesh
{
public:

	Primitive(PrimitiveType type);
	~Primitive();
};
