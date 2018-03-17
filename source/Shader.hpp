#pragma once

class Mesh;
class Scene;
class Material;
class Transform;

class Shader
{
public:

	unsigned int id;

	unsigned int viewID;
	unsigned int projectionID;
	unsigned int modelID;

	unsigned int materialShininessID;
	unsigned int materialSpecularColorID;
	unsigned int lightPositionID;
	unsigned int lightIntensitiesID;
	unsigned int lightAttenuationID;
	unsigned int lightAmbientCoefficientID;
	unsigned int cameraPositionID;

	Shader();

	void SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material);

	static unsigned int activeShader;
};

