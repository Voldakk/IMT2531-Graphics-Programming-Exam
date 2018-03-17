#include "Primitive.hpp"

#include <string>

Primitive::Primitive(PrimitiveType type)
{
	std::string path = "../assets/primitives/";

	switch (type)
	{
	case Cube:
		path += "cube.obj";
		break;
	default:
		break;
	}

	Mesh(path);
}

Primitive::~Primitive()
{

}