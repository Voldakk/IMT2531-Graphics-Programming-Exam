#version 330 core

uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;
layout (location = 3) in mat4 instanceMatrix;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;
out mat4 model;

void main() 
{
    // Pass some variables to the fragment shader
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    fragVert = vert;
    model = instanceMatrix;

    // Apply all matrix transformations to vert
    gl_Position = projection * view * instanceMatrix * vec4(vert, 1);
}