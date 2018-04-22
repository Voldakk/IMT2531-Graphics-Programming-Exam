#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 fragUV;

uniform vec2 position;
uniform vec2 size;

uniform mat4 projection;

void main()
{
	vec2 vertPos = (pos*size) + position;
    gl_Position = projection * vec4(vertPos, 0.0, 1.0);
    fragUV = uv;
}  