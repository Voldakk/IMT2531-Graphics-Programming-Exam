#version 150

in vec4 position;
in vec2 texcoord;
out vec2 Texcoord;

uniform mat4 projection;
uniform mat4 translation = mat4(1.0);
uniform mat4 rotation  = mat4(1.0);
uniform mat4 scale = mat4(1.0);

void main()
{
	Texcoord = texcoord;
	gl_Position = projection * translation * scale * rotation * position;
}