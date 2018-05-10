#version 330 core
layout (location = 0) in vec3 vert;
layout (location = 5) in mat4 instanceMatrix;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * instanceMatrix * vec4(vert, 1.0);
}  