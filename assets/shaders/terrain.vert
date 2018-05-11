#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

// Lights
#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 color;
   float attenuation;
   float ambientCoefficient;
   mat4 lightSpaceMatrix;
   float farPlane;

} allLights[MAX_LIGHTS];

out vec4 allFragPosLightSpace [MAX_LIGHTS];

void main() 
{
    // Pass some variables to the fragment shader
	fragVert = vec3(model * vec4(vert, 1));
    fragTexCoord = vertTexCoord;
    fragNormal = mat3(transpose(inverse(model))) * vertNormal;  
    
	for(int i = 0; i < numLights; ++i)
    {
        allFragPosLightSpace[i] = allLights[i].lightSpaceMatrix * vec4(fragVert, 1.0);
    }

    // Apply all matrix transformations to vert
    gl_Position = projection * view * model * vec4(vert, 1);
}