#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;
layout (location = 3) in vec3 vertTangent;
layout (location = 4) in vec3 vertBitangent;  

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;
out mat3 fragTBN;

out vec4 fragPosLightSpace;

void main() 
{
    // Pass some variables to the fragment shader
	fragVert = vec3(model * vec4(vert, 1));
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    
	fragPosLightSpace = lightSpaceMatrix * vec4(fragVert, 1.0);

    // Apply all matrix transformations to vert
    gl_Position = projection * view * model * vec4(vert, 1);

	vec3 T = normalize(vec3(model * vec4(vertTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(vertBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(vertNormal,    0.0)));
	fragTBN = mat3(T, B, N);
}