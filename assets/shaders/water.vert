#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

uniform float waveLength;
uniform float amplitude;
uniform float verticesPerUnit;

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;

out vec3 fragVert;
out vec3 fragNormal;
out vec2 fragTexCoord;

// Lights
#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 color;
   float attenuation;
   float ambientCoefficient;
   sampler2D shadowMap;
   mat4 lightSpaceMatrix;
   float farPlane;

} allLights[MAX_LIGHTS];

out vec4 allFragPosLightSpace [MAX_LIGHTS];

float GetHeight(float x, float z)
{
	float height = (sin((x + time) / waveLength) + sin((z + time) / waveLength)) * amplitude;
	return height;
}

void main() 
{
	// Position
	vec3 vertPos =  vert;
	vertPos.y = GetHeight(vert.x, vert.z);
	
	fragVert = vec3(model * vec4(vertPos, 1));

	// Normal
	float xm = GetHeight(vert.x - (1/verticesPerUnit), vert.z);
	float xp = GetHeight(vert.x + (1/verticesPerUnit), vert.z);
	float zm = GetHeight(vert.x, vert.z - (1/verticesPerUnit));
	float zp = GetHeight(vert.x, vert.z + (1/verticesPerUnit));

	// Deduce normal
	fragNormal.x = xm - xp;
	fragNormal.z = zm - zp;
	fragNormal.y = 2.0;
	fragNormal = normalize(fragNormal);

	// UV
    fragTexCoord = vertTexCoord;
    
	// Light space
	for(int i = 0; i < numLights; ++i)
    {
        allFragPosLightSpace[i] = allLights[i].lightSpaceMatrix * vec4(fragVert, 1.0);
    }

    // Apply all matrix transformations to vert
    gl_Position = projection * view * model * vec4(vertPos, 1);
}