#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main()
{
	// Pass some variables to the fragment shader
    fragTexCoord = vertTexCoord;

    // Apply all matrix transformations to vert
    gl_Position = projection * view * model * vec4(vert, 1);
}