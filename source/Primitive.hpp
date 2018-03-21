#pragma once

#include "Mesh.hpp"

enum PrimitiveType { Circle, Cone, Cube, CubeInverted, Cylinder, Icosphere, Monkey, MonkeyHigh, Plane, Sphere, Torus };

class Primitive : public Mesh
{
public:

	explicit Primitive(PrimitiveType type);
};

