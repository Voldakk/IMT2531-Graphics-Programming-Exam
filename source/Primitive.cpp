#include "Primitive.hpp"

#include <string>

Primitive::Primitive(PrimitiveType type)
{
	std::string path = "./assets/primitives/";

	switch (type)
	{
	case Circle:
		path += "circle.obj";
		break;
	case Cone:
		path += "cone.obj";
		break;
	case Cube:
		path += "cube.obj";
		break;
	case CubeInverted:
		path += "cube_inverted.obj";
		break;
	case Cylinder:
		path += "cylinder.obj";
		break;
	case Monkey:
		path += "monkey_low.obj";
		break;
	case MonkeyHigh:
		path += "monkey_high.obj";
		break;
	case Icosphere:
		path += "icosphere.obj";
		break;
	case Plane:
		path += "plane.obj";
		break;
	case Sphere:
		path += "sphere.obj";
		break;
	case Torus:
		path += "torus.obj";
		break;
		// Default to cube
	default:
		path += "cube.obj";
		break;
	}

	LoadMesh(path.c_str());
}