#pragma once

class Mesh;
class Scene;
class Material;
class Transform;

class Shader
{
public:

	unsigned int id = -1;
	Shader();

	virtual void SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material);

	static unsigned int activeShader;
};

class StandardShader : public Shader
{
public:
	unsigned int viewId;
	unsigned int projectionId;
	unsigned int modelId;

	unsigned int materialShininessId;
	unsigned int materialSpecularColorId;
	unsigned int lightPositionId;
	unsigned int lightIntensitiesId;
	unsigned int lightAttenuationId;
	unsigned int lightAmbientCoefficientId;
	unsigned int cameraPositionId;

	StandardShader();

	void SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material) override;
};

class UnlitTextureShader : public Shader
{
public:

	unsigned int viewId;
	unsigned int projectionId;
	unsigned int modelId;

	UnlitTextureShader();

	void SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material) override;
};

class SkyboxShader : public Shader
{
public:

	unsigned int viewId;
	unsigned int projectionId;
	unsigned int modelId;

	unsigned int textureId;

	SkyboxShader();

	void SetUniforms(Scene * scene, Transform * transform, Mesh * mesh, Material * material) override;
};