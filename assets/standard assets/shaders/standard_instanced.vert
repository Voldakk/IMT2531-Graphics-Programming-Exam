#version 330 core

uniform mat4 lightSpaceMatrix;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;
layout (location = 3) in vec3 vertTangent;
layout (location = 4) in vec3 vertBitangent;  
layout (location = 5) in mat4 instanceMatrix;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;
out mat3 fragTBN;
out mat4 model;

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

void main() 
{
    // Pass some variables to the fragment shader
	model = instanceMatrix;

	fragVert = vec3(model * vec4(vert, 1));
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    
	for(int i = 0; i < numLights; ++i)
    {
        allFragPosLightSpace[i] = allLights[i].lightSpaceMatrix * vec4(fragVert, 1.0);
    }

    // Apply all matrix transformations to vert
    gl_Position = projection * view * instanceMatrix * vec4(vert, 1);

	// Calculate TBN matrix
	vec3 T = normalize(vec3(model * vec4(vertTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(vertBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(vertNormal,    0.0)));
	fragTBN = mat3(T, B, N);
}