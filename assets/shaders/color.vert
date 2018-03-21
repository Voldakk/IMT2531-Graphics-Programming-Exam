#version 150

in vec4 position;
in vec4 color;
out vec4 color_from_vshader;

uniform mat4 projection;

void main() {
	gl_Position = projection * position;
	color_from_vshader = color;
}